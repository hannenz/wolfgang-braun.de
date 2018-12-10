<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class postRelationsController .. Local Class extends Controller 
 */
class postRelationsController extends Controller {

	protected $postId;
	protected $categoryId;
	protected $currentPage;
	protected $templatePath;
	protected $relations;
	protected $postRelations;
	private $cmt;

	public function init() {
		$this->cmt = Contentomat::getContentomat();
		$this->load(PATHTOADMIN . 'classes/class_relations_view.inc');
		$this->postId = $this->getvars['aid']; // article id
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->categoryId = $this->getvars['cp']; // current page
		$this->templatePath = $this->templatesPath . 'widgets/post_relations/';  // replace with 
		$this->initRelations();
	}

	protected function initActions($action = '') {

		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'showMedia';
		}
	}

	protected function initRelations() {
		if (!$this->postId) {
			return false;
		}

		$this->relations = new Relations();

		// table id => shortname
		$this->relations->tableTypWrapper = array(
			"25" => "mlog"
		);

		$this->postRelations = $this->relations->getRelations($this->pageId, $this->postId, 'mlog_posts', 'post_relations');
	}

	/*	 * ************************************
	  A C T I O N S
	 * ************************************ */

	protected function actionShowMedia() {

		if (!is_array($this->postRelations) || empty($this->postRelations)) {
			return false;
		}
		
		$relationListContent = '';
		
		foreach ($this->postRelations as $relPost) {
			$this->parser->setMultipleParserVars($relPost);
			$this->parser->setParserVar('linkTitle',  $this->cmt->makeNameWebsave($relPost['post_title']));
			$relationListContent .= $this->parser->parseTemplate($this->templatePath . 'relations_row.tpl');
		}

		if($relationListContent){
			$this->parser->setParserVar('relationListContent',$relationListContent);
			$this->content = $this->parser->parseTemplate($this->templatePath.'relations_frame.tpl');
		}
		
	}

}

$controller = new postRelationsController();

$content .= $controller->work();
?>

