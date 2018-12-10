<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class mostCommentedController .. Local Class extends Controller 
 */
class mostCommentedController extends Controller {

	protected $postId;
	protected $categoryId;
	protected $templatePath;
	protected $posts;
	protected $postData;
	protected $postsList;
	protected $postsViewLimit = 3;	// limit number of posts to return
	protected $postsDateLimit = 30; // show posts from last ## days

	public function init() {
		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
		$this->postId = $this->getvars['aid']; // article id
		$this->categoryId = $this->getvars['cat']; // article category id


		$this->templatePath = $this->templatesPath . 'widgets/most_commented/';  // replace with 
		$this->initPosts();
	}

	protected function initActions($action = '') {

		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'showMostCommented';
		}
	}

	protected function initPosts() {
		$this->posts = new Posts();

		$condWhere = array();


		$condWhere[] = "post_online_date BETWEEN DATE_SUB(NOW(), INTERVAL ".$this->postsDateLimit." DAY) AND NOW()";


		$posts = $this->posts->search(array(
			'orderDir' => 'DESC',
			'orderBy' => 'post_online_date',
			'condWhere' => $condWhere,
			'entriesPerPage' => 99999,
			'currentPage' => 1,
			'testFlag' => 1
				));

		$this->postsList = $this->sortMultiArrayByFieldValue($posts, 'commentsNr', 'desc');
		
	}

	protected function sortMultiArrayByFieldValue($sourceArray, $sortby, $direction = 'asc') {

		$sortedArr = array();
		$tmp_Array = array();

		foreach ($sourceArray as $key => $value) {
			$tmp_Array[$key] = strtolower($value[$sortby]);
		}

		if ($direction == 'asc') {
			asort($tmp_Array);
		} else {
			arsort($tmp_Array);
		}

		foreach ($tmp_Array as $key => $fieldValue) {
			if (!$fieldValue) {
				continue;
			}
			$sortedArr[] = $sourceArray[$key];
		}

		return array_slice($sortedArr,0,$this->postsViewLimit);
	}

	/*	 * ************************************
	  A C T I O N S
	 * ************************************ */

	protected function actionShowMostCommented() {

		if (!is_array($this->postsList) || empty($this->postsList)) {
			return false;
		}

		$mostCommentedContent = '';

		foreach ($this->postsList as $post) {
			$this->parser->setMultipleParserVars($post);
			$mostCommentedContent .= $this->parser->parseTemplate($this->templatePath . 'most_commented_row.tpl');
		}

		$this->parser->setParserVar('mostCommentedContent', $mostCommentedContent);
		$this->content = $this->parser->parseTemplate($this->templatePath . 'most_commented_frame.tpl');
	}

}

$controller = new mostCommentedController();

$content .= $controller->work();
?>

