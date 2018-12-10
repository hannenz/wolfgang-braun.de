<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class postMediaController .. Local Class extends Controller 
 */
class postMediaController extends Controller {
	
	protected $postId;
	
	protected $categoryId;
	protected $currentPage;
	protected $templatePath;
	protected $posts;
	protected $postData;
	protected $elementSeparator;
	

	public function init() {
		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
		$this->postId = $this->getvars['aid']; // article id
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->categoryId = $this->getvars['cp']; // current page
		$this->templatePath = $this->templatesPath . 'widgets/post_media/';  // replace with 
		
		$this->elementSeparator = '&nbsp;/ ';
		
		$this->initPost();
		
	}

	protected function initActions($action = '') {
		
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'showMedia';
		}
	}
	
	
	protected function initPost() {
		$this->posts = new Posts();
		
		if($this->postId){
			$this->postData = $this->posts->getPost(array('postID'=>$this->postId));
		}else{
			$this->action = '';
		}

	}
	
	/**************************************
				A C T I O N S 
	 **************************************/

	protected function actionShowMedia(){
		if (!is_array($this->postData) || empty($this->postData)) {
			return false;
		}
		
		$this->parser->setMultipleParserVars($this->postData['hasMedia']);

		$mediaContent = array();


		if ($this->postData['postMedia']) {
			foreach ($this->postData['postMedia'] as $mediaType => $mediaItems) {

				foreach ($mediaItems as $media) {
					$this->parser->setParserVar('mediaType', strtolower($mediaType));
					$this->parser->setMultipleParserVars($media);
					$mediaContent[$mediaType][] = $this->parser->parseTemplate($this->templatePath . 'media_row.tpl');
				}
			}
		}

		if ($mediaContent) {
			foreach ($mediaContent as $mediaType => $mediaContent) {
				$this->parser->setParserVar('contentList' . $mediaType, join($this->elementSeparator, $mediaContent));
			}
			
			$this->content = $this->parser->parseTemplate($this->templatePath.'media_frame.tpl');
		}

	}
}

$controller = new postMediaController();

$content .= $controller->work();
?>

