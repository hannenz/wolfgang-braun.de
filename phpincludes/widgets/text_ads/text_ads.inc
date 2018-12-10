<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class TextAdsController .. Local Class extends Controller 
 */
class TextAdsController extends Controller {
	
	protected $categoryId;
	protected $currentPage;
	protected $templatePath;
	protected $textAds;

	public function init() {
		$this->load($this->phpClassesPath . 'class_text_ads.inc');
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->categoryId = $this->getvars['cp']; // current page
		$this->templatePath = $this->templatesPath . 'widgets/text_ads/';  // replace with 
		$this->textAds = new TextAds();
	}

	protected function initActions($action = '') {
		
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'showTextAds';
		}
	}
	
	
	protected function actionShowTextAds(){
		$ads = $this->textAds->getAds();
		
		$adsContent = '';
		foreach($ads as $ad){
			$this->textAds->increaseViews($ad['id']);
			$this->parser->setMultipleParserVars($ad);
			$adsContent .= $this->parser->parseTemplate($this->templatePath.'widget_text_ads_row.tpl');
		}
		
		$this->parser->setParserVar('adsContent',$adsContent);
		$this->content = $this->parser->parseTemplate($this->templatePath.'widget_text_ads_frame.tpl');
	}



	/**************************************
				A C T I O N S 
	 **************************************/

}

$controller = new TextAdsController();

$content .= $controller->work();
?>

