<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class similarPostsController .. Local Class extends Controller 
 */
class similarPostsController extends Controller {

	protected $postId;
	protected $categoryId;
	protected $currentPage;
	protected $templatePath;
	protected $posts;
	protected $postData;
	protected $elementSeparator;
	protected $similarPostsLimit;
	private $cmt;

	public function init() {
		
		$this->cmt = Contentomat::getContentomat();
		
		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
		$this->postId = $this->getvars['aid']; // article id
		$this->categoryId = intval($this->getvars['cat']); // article category id
		$this->currentPage = $this->getvars['cp']; // current page
		$this->templatePath = $this->templatesPath . 'widgets/similar_posts/';  // replace with 

		$this->elementSeparator = '&nbsp;/ ';
		$this->similarPostsLimit = 5;

		$this->initPost();
	}

	protected function initActions($action = '') {

		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'showSimilarPosts';
		}
	}

	protected function initPost() {
		$this->posts = new Posts();


		if ($this->postId) {
			$this->postData = $this->posts->getPost(array('postID' => $this->postId));
		} else {
			$this->action = '';
		}
	}

	
	protected function initRecentCategoryPosts(){
		
		if(!$this->categoryId){
			$this->categoryId = $this->postData['post_categories'][0]['id'];
		}
		
		$condWhere[] = $condWhere[] = "id <> ".$this->postId;
		
		$returnPosts = $this->posts->search(array(
			'orderDir' => 'DESC',
			'orderBy' => 'post_online_date',
			'condWhere' => $condWhere,
			"entriesPerPage" => $this->similarPostsLimit,
			'currentPage' => $this->currentPage,
			'categoryLimit' => $this->categoryId
		));
		
		return $returnPosts;
	}
	/*	 * ************************************
	  A C T I O N S
	 * ************************************ */

	protected function actionShowSimilarPosts() {
		if (!is_array($this->postData) || empty($this->postData)) {
			return false;
		}

		$tags = explode(",", $this->postData['post_tags']);

		if ($tags[0]) {
			$searchTag = trim($tags[0]);
		}

		if ($searchTag) {
			$similarListContent = '';
			
			$similarPosts = $this->posts->getSimilarPostsByTags(array(
				'searchTag' => $searchTag,
				'currentArticleId'=>$this->postId,
				"limit" => $this->similarPostsLimit,
					));
			
			if(empty($similarPosts)){
				$similarPosts = $this->initRecentCategoryPosts($this->postData['id']);
			}
			
			foreach ($similarPosts as $post) {
				$this->parser->setMultipleParserVars($post);
				$this->parser->setParserVar('linkTitle', $this->cmt->makeNameWebsave($post['post_title']));
				$similarListContent .= $this->parser->parseTemplate($this->templatePath . 'similar_posts_row.tpl');
			}

			if ($similarListContent) {
				$this->parser->setParserVar('similarListContent', $similarListContent);
				$this->content = $this->parser->parseTemplate($this->templatePath . 'similar_posts_frame.tpl');
			}
		}

	}

}

$controller = new similarPostsController();

$content .= $controller->work();
?>

