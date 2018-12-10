<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class TagsController .. Local Class extends Controller 
 */
class TagsController extends Controller {
	
	protected $categoryId;
	protected $currentPage;
	protected $templatePath;
	protected $tagsCloud;

	public function init() {
		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_tag.inc');
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->categoryId = $this->getvars['cp']; // current page
		$this->templatePath = $this->templatesPath . 'widgets/tags_cloud/';  // replace with 
		$this->initTags();
	}

	protected function initActions($action = '') {
		
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			 $this->action = 'showTags';
		}
	}

	
	protected function initTags(){
		$this->tags = new Tag();
		$this->tagsCloud = $this->tags->getTagsCloud();
	}
	/**************************************
				A C T I O N S 
	 **************************************/
  public function actionShowTags(){
	  $tagCloudContent = array();
		
		if(is_array($this->tagsCloud) && !empty($this->tagsCloud)){
			foreach($this->tagsCloud as $tag){
				$link = "/".PAGELANG."/10/Themen.html?searchIn=tags&search=".$tag['tag'];
				
				$tagCloudContent[] = array(
					'text'=>$tag['tag'],
					'weight'=>$tag['fontSize'],
					'link'=>$link,
					'title'=>$tag['tag'],
					'html'=>'"class":"tag"'
				);
			}
		}
		
		$this->parser->setParserVar('tagCloudContent',json_encode($tagCloudContent));
		$this->content = $this->parser->parseTemplate($this->templatePath.'widget_tags.tpl');
  }
  
}

$controller = new TagsController();

$content .= $controller->work();
?>

