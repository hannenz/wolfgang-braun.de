<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class InfoController .. Local Class extends Controller 
 */
class InfoController extends Controller {

	protected $postId;
	protected $categoryId;
	protected $currentPage;
	protected $templatePath;
	protected $posts;
	protected $authors;
	protected $elementSeparator;
	private $cmt;

	public function init() {
		
		$this->cmt = Contentomat::getContentomat();
		
		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
		$this->load($this->phpClassesPath . 'class_authors.inc');
		
		$this->postId = $this->getvars['aid']; // article id
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->currentPage = $this->getvars['cp']; // current page
		$this->templatePath = $this->templatesPath . 'widgets/post_infos/';  // replace with

		$this->elementSeparator = '&nbsp;/ ';
		
		$this->authors = new Authors();
		$this->posts = new Posts();
	}

	protected function initActions($action = '') {

		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {

			$this->action = 'showArticleInfo';
		}
	}

	protected function actionShowArticleInfo() {
		$articleInfo = array();
		if ($this->postId) {
			$this->content = $this->postId;
			$post = $this->posts->getPost(array('postID' => $this->postId));
			
			// Autor ermitteln
			if ($post['post_author']) {
				$author = $this->authors->getAuthor(array('authorId' => $post['post_author']));
				if(!empty($author)){
					$author['authorLinkName'] = $this->cmt->makeNameWebsave($author['author_prename'].' '.$author['author_name']);
				}
			}
			
			// Kategorien ermitteln
			$categoriesArray = array();
			if (is_array($post['post_categories'])) {
				foreach ($post['post_categories'] as $postCategory) {
					$this->parser->setMultipleParserVars($postCategory);
					$this->parser->setParserVar('categoryName', ucfirst($postCategory['category_name']));
					$categoriesArray[] = trim($this->parser->parseTemplate($this->templatePath . 'post_category.tpl'));
				}
			}
			$categoriesContent = implode($this->elementSeparator, $categoriesArray);
			$categoriesNr = count($post['post_categories']);

		}
		
		$tagsContentArray = array();
		$tagsContent = array();
		
		if($post['post_tags']){
			$tags = explode(",",$post['post_tags']);
			foreach($tags as $tag){
				if(trim($tag)){
					$this->parser->setParserVar('tag',trim($tag));
					$tagsContentArray[] = trim($this->parser->parseTemplate($this->templatePath.'post_tag.tpl'));
				}
			}
			
			$tagsContent = implode($this->elementSeparator, $tagsContentArray);
		}

		$this->parser->setMultipleParserVars($post);
		$this->parser->setMultipleParserVars($author);
		$this->parser->setParserVar('tagsContent',$tagsContent);
		$this->parser->setParserVar('categoriesContent',$categoriesContent);
		$this->parser->setParserVar('categoriesNr',$categoriesNr);
		$this->parser->setParserVar('currentPage',$this->currentPage);
		$this->parser->setParserVar('categoryId',$this->categoryId);
		$this->parser->setParserVar('postId',$post['id']);
		$this->parser->setParserVar('authorId',$author['id']);
		$this->content = $this->parser->parseTemplate($this->templatePath.'post_infos_frame.tpl');
	}

	/*	 * ************************************
	  A C T I O N S
	 * ************************************ */
}

$controller = new InfoController();

$content .= $controller->work();
?>

