<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class mlogView .. Local Class extends Controller 
 */
class WidgetsController extends Controller {

	protected $channelId;
	protected $channelData;
	protected $categoryId;
	protected $templatePath;

	public function init() {
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->templatePath = $this->templatesPath . 'widgets/';
	}

	/**
	 *
	 * @param type $action 
	 */
	protected function initActions($action = '') {
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			// Show Category widget channel 
			// where the main channel of page is Mlog && Category is given
			if ($this->channelData['channel_title'] == 'Mlog' && $this->categoryId) {
				$this->action = 'showMlogWidgets';
			
			// Show Page widgets channel	
			} else {
				$this->action = 'showPageWidgets';
			}
		}
	}

	/**
	 * public function initWidgets()
	 * 
	 * set variables and active action
	 * 
	 * @param array $cmt_content, current content global variables
	 */
	public function initWidgets($cmt_content) {
		$this->cmtContent = $cmt_content;
		$this->channelId = $this->cmtContent['head1'];
		$channelData = $this->channelData = $this->getChannelData();
		if (!empty($channelData)) {
			$this->channelData = $channelData;
		}
		$this->initActions();
	}

	/**
	 * protected function getChannelData()
	 * 
	 * get data of current channel
	 * 
	 * @return array 
	 */
	protected function getChannelData() {
		$this->db->query("SELECT * FROM widgets_channels WHERE id='" . $this->channelId . "'");
		$res = $this->db->get();
		if (is_array($res)) {
			return $res;
		}
		return array();
	}

	/**
	 * protected function getChannelWidgets()
	 * 
	 * parse current channel widgets
	 * 
	 * @return string , html content of channel widgets
	 */
	protected function getChannelWidgets() {
		
		$widgets = array();
		
		$widgetsContent = '';
		
		$widgetsIds = $this->channelData['widget_ids'];

		$widgetsIds = explode(",", $widgetsIds);

		if (!is_array($widgetsIds)) {
			return null;
		}

		foreach ($widgetsIds as $widgetId) {
			$this->db->query("SELECT * FROM widgets WHERE id='" . $widgetId . "'");
			$res = $this->db->get();
			if (is_array($res)) {
				$widgets[]=$res;
			}
		}
		
		$lastWidget = count($widgets)-1;
		foreach($widgets as $index=>$widget){
			$currentWidgetContent = '';
			$currentWidgetContent = $this->parser->parse($widget['widget_html']);
			$this->parser->setParserVar('widgetContent',$currentWidgetContent);
			$this->parser->setParserVar('firstWidget',$index==0);
			$this->parser->setParserVar('lastWidget',$index==$lastWidget);
			if(trim($currentWidgetContent)){
				$widgetsContent .= $this->parser->parseTemplate($this->templatePath.'widget_box.tpl');
			}
			
		}
		return $widgetsContent;
	}
	
	
	/**************************************
				A C T I O N S 
	 **************************************/
	
	/**
	 * protected function actionShowPageWidgets()
	 * 
	 * Action: show current page widgets
	 * 
	 * @return void
	 */
	protected function actionShowPageWidgets() {
		$widgetsContent = $this->getChannelWidgets();
		$this->content = $widgetsContent;
	}

	/**
	 * protected function actionShowMlogWidgets()
	 * 
	 * Action: show selectec mlog category widgets
	 * 
	 * @return void 
	 */
	protected function actionShowMlogWidgets() {
		$this->db->query("SELECT * FROM widgets_channels WHERE mlog_category='".$this->categoryId."'");
		
		$res = $this->db->get();
		
		if(is_array($res)){
			$this->channelData = $res;
		}
		
		$widgetsContent = $this->getChannelWidgets();
		$this->content = $widgetsContent;
		
	}

}

$controller = new WidgetsController();

$controller->initWidgets($cmt_content);

$content .= $controller->work();
?>

