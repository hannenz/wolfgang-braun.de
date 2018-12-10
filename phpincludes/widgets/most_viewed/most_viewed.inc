<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class mlogView .. Local Class extends Controller 
 */
class MostViewedController extends Controller {
	
	protected $categoryId;
	protected $templatePath;
	protected $lastDaysLimit;
	protected $mostViewedArticlesLimit;
	protected $categoryData;
	protected $condWhere;

	public function init() {
		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->templatePath = $this->templatesPath . 'widgets/most_viewed/';
		$this->lastDaysLimit=30;
		$this->mostViewedArticlesLimit=3;
		$this->condWhere = array();
		$this->initPosts();
		
	}

	protected function initActions($action = '') {
		
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'showMostViewedArticles';
		}
	}

	
	protected function initPosts() {
		$this->posts = new Posts();
		
		if($this->categoryId){
			$this->categoryData = $this->posts->getCategoryData($this->categoryId);
		}
	}
	
	/**************************************
				A C T I O N S 
	 **************************************/
	protected function actionShowMostViewedArticles() {
		
		$this->condWhere[] = "post_online_date BETWEEN CURDATE() - INTERVAL ".$this->lastDaysLimit." DAY AND CURDATE()";
		
		$posts = $this->posts->search(array(
			'orderDir' => 'DESC',
			'orderBy' => 'post_views',
			'condWhere' => $this->condWhere,
			"teaserFieldPriority" => 'post_text',
			"entriesPerPage" => $this->mostViewedArticlesLimit,
			'pagingShowLinks' => $this->pagingLinks,
			'currentPage' => 1,
			'categoryLimit' => $this->categoryId
			));
		$mostViewedContent='';
		
		foreach($posts as $post){
			$this->parser->setMultipleParserVars($post);
			$this->parser->setParserVar('categoryId',$this->categoryData['id']);
			$mostViewedContent .= $this->parser->parseTemplate($this->templatePath.'most_viewed_row.tpl');
		}
		
		$this->parser->setParserVar('categoryTitle',$this->categoryData['category_title_de']);
		$this->parser->setParserVar('mostViewedContent',$mostViewedContent);
		$this->content = $this->parser->parseTemplate($this->templatePath.'most_viewed_frame.tpl');
		
		
	}

}

$controller = new MostViewedController();

$content .= $controller->work();
?>

