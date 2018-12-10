<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class mlogView .. Local Class extends Controller 
 */
class MlogController extends Controller {

	protected $postId;
	protected $categoryId;
	protected $categoryData;
	protected $searchTextRequest;
	protected $searchIn;
	protected $mlog;
	protected $posts;
	protected $comments;
	protected $mlogTemplatePath;
	protected $commentsTemplatePath;
	protected $adminActions;
	protected $actionHash;
	protected $adminEmail;
	protected $commentVars;
	protected $showNewCommentForm;
	protected $commentsFieldsWrapper;
	protected $currentPage;
	protected $entriesPerPage = 10;
	protected $pagingLinks = 10;
	protected $authors;
	protected $postRelations;
	protected $overviewAllowedCategories;

	public function init() {

		$this->load(PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
		$this->load(PATHTOADMIN . 'classes/class_paging.inc');
		$this->load($this->phpClassesPath . 'class_authors.inc');
		$this->load($this->phpClassesPath . 'class_comments.inc');
		$this->load(PATHTOADMIN . 'classes/class_relations_view.inc');

		// templates path
		$this->mlogTemplatePath = $this->templatesPath . 'mlog/';
		$this->commentsTemplatePath = $this->templatesPath . 'comments/';


		// properties
		$this->postId = $this->getvars['aid']; // article id
		$this->categoryId = $this->getvars['cat']; // article category id
		$this->searchTextRequest = $this->getvars['search']; // such begriff
		$this->searchIn = $this->getvars['searchIn']; // such begriff
		$this->adminActions = $this->getvars['act'];
		$this->actionHash = $this->getvars['hash'];
		$this->currentPage = intval(trim($this->getvars['cp']));
		if (!$this->currentPage) {
			$this->currentPage = 1;
		}

		$this->showNewCommentForm = true;
		$this->commentsFieldsWrapper = array('author', 'commentTitle', 'email', 'author_url', 'comment', 'notify', 'captchaVarName');
		$this->errorMessage = array();

		// init main parts
		$this->initActions();
		$this->initPosts();
		$this->initRelations();
		$this->initComments();
	}

	protected function initActions($action = '') {
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			// define  actions
			if ($this->postId) {
				$this->action = 'showPostDetails';
			} else {
				$this->action = 'showPostsList';
			}
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
	
	


	/*
	 * @return bool
	 */

	protected function initComments() {

		$commentPostId = intval($this->postvars['comment_post_ID']);

		foreach ($this->commentsFieldsWrapper as $field) {
			$this->commentVars[$field] = strip_tags($this->postvars[$field]);
		}
		$this->commentVars['comment_post_ID'] = $commentPostId;

		// comments instance
		$this->comments = new Comments();

		// the default avatar for anonymouse users comments
		$default_avatar_path = 'http://' . $_SERVER['HTTP_HOST'] . '/img/default_avatar.png';

		// admin email, used to notification or activation
		//$this->adminEmail = "test@buero-hahn.de";

		// comments needing admin activation 
		$commentsRequestAdminActivation = false;

		//default show add new comment form
		$showNewCommentForm = true;

		$showCommentsInListView = false;


		$this->comments->init(
				array(
					"comments_table" => "mlog_comments",
					"page_id" => $this->pageId,
					"article_id" => $this->postId,
					"template_row" => $this->commentsTemplatePath . 'comments_row.tpl',
					"template_frame" => $this->commentsTemplatePath . 'comments_frame.tpl',
					"default_avatar_path" => $default_avatar_path,
					"commentsRequestAdminActivation" => $commentsRequestAdminActivation,
		));

		// admin actions to delete or activate comments with one click link

		if ($this->postvars['submit_form']) {
			$this->saveComment();
		}

		if ($this->adminActions == 'activate' && $this->actionHash) {
			$this->activateComment();
		}

		if ($this->adminActions == 'delete' && $this->actionHash) {
			$this->deleteComment();
		}
		
		if($this->comments->errorNr){
			$this->parser->setParserVar('errorNr',$this->comments->errorNr);
			$this->errorMessage['commentMessage'] = $this->parser->parseTemplate($this->commentsTemplatePath.'error_messages.tpl');
		}
		
	}

	/*
	 * @return bool
	 */

	protected function saveComment() {

		if ($this->postvars['submit_form']) {
			$check = $this->comments->isValid($this->commentVars);
			
			if ($check) {
				
				// notify admin about new comment, to activated or delete it (activate only if $commentsRequestAdminActivation is true)
				$this->comments->notifyAdministrator(array("adminEmail" => $this->adminEmail, "template" => $this->commentsTemplatePath . 'notify_admin.tpl'));

				// notify authors about comment answers if you checked the option (notify) in form
				$this->comments->notifyFollowers(array("userEmail"=>$this->commentVars['email'],"template" => $this->commentsTemplatePath . 'notify_followers.tpl'));

				$this->showNewCommentForm = false;
				
				$this->comments->resetCaptcha();
				
				$this->errorMessage = array();
				
			} else {
				$this->parser->setParserVar('errorNr', $this->comments->errorNr);
				$this->parser->setMultipleParserVars($this->postvars);
				$this->errorMessage['commentMessage'] = $this->parser->parseTemplate($this->commentsTemplatePath . 'error_messages.tpl');
			}

		}
	}

	/*
	 * one click link, activate comment by clicking activate link sended to admin
	 * 
	 */

	protected function activateComment() {
		$this->comments->activateComment(array('hash' => $this->actionHash));
	}

	/**
	 *  one click link, delete comment by clicking a link sended to admin
	 */
	protected function deleteComment() {
		$this->comments->deleteComment(array('hash' => $this->actionHash));
	}

	protected function initPosts() {
		$this->posts = new Posts();
		$this->authors = new Authors();

		$alternateTemplatePath = trim(html_entity_decode($this->cmtContent['head2']), "\xA0");

		if ($alternateTemplatePath != '') {
			$this->mlogTemplatePath = $this->templatesPath . $alternateTemplatePath . '/';
		}

		if ($this->categoryId) {
			$this->categoryData = $this->posts->getCategoryData($this->categoryId);
		}
		
		
		// get allowd categories in overview mode
		$mCategoreis = $this->posts->getPostsCategories();
		
		foreach($mCategoreis as $mCat){
			if($mCat['category_overview']){
				$this->overviewAllowedCategories[] = $mCat['id'];
			}
		}
		
	}

	/**
	 * ACTIONS
	 */
	protected function actionShowPostsList() {

		if($this->categoryData){
			define('POSTSCATEGORY',': '.$this->categoryData['category_title_de'] );
		}
		
		$condWhere = array();
		$searchFields = 'post_text,post_tags,post_title';

		if ($this->searchTextRequest != '') {
			$searchText = $this->searchTextRequest;
			$searchMode = True;
		}


		if ($this->searchIn != '') {

			switch ($this->searchIn) {
				case 'tags':
					$searchFields = 'post_tags';
					break;
				case 'authors':
					$searchFields = 'post_author';
					$condWhere[] = "post_author='" . $this->searchTextRequest . "'";
					break;
			}
		}

		// show only allowed categories articles in articles list overview
		if(!$searchText && !$this->categoryId){
			$allowedCategoriesQueryFragments = array();
			foreach($this->overviewAllowedCategories as $allowedCategory){
				$allowedCategoriesQueryFragments[] = "FIND_IN_SET ('".$allowedCategory."',mlog_posts.post_category)";
			}
			
			$condWhere[] = "(".join(" OR ",$allowedCategoriesQueryFragments).")";
		}
		
		

		// search in category
		//if ($this->categoryId) {
		//	$condWhere = "FIND_IN_SET('" . $this->categoryId . "',mlog_posts.post_category)";
		//}

		$posts = $this->posts->search(array(
			'orderDir' => 'DESC',
			'orderBy' => 'post_online_date',
			'condWhere' => $condWhere,
			//'condWhere' => $condWhere,
			"searchText" => $searchText,
			"searchMode" => $searchMode,
			"searchFields" => $searchFields,
			"searchType" => 'like',
			"teaserFieldPriority" => 'post_text',
			"entriesPerPage" => $this->entriesPerPage,
			'pagingShowLinks' => $this->pagingLinks,
			'currentPage' => $this->currentPage,
			'categoryLimit' => $this->categoryId
				));


		if (!empty($posts)) {

			foreach ($posts as $post) {


				// total comments in article
				$this->parser->setParserVar('commentsTotal', $this->comments->getCommentsCountOfArticle($post['id']));

				// the last 3 comments in article
				$lastComments = $this->comments->getLastCommentsOfArticle($post['id'], 3);

				// last comments rows view
				$lastCommentsList = '';

				foreach ($lastComments as $comment) {
					$this->parser->setMultipleParserVars($comment);
					$lastCommentsList .= $this->parser->parseTemplate($this->commentsTemplatePath . 'last_comments.tpl');
				}



				if ($showCommentsInListView == true) {
					if ($showNewCommentForm || $post['id'] != $commentPostId) {
						if ($this->errorMessage[$post['id']]) {
							$lastCommentsList .= $this->errorMessage[$post['id']];
						}
						$this->parser->setParserVar('author', trim($commentAuthor[$post['id']]));
						$this->parser->setParserVar('commentTitle', $commentTitle[$post['id']]);
						$this->parser->setParserVar('title_url', Contentomat::makeNameWebsave($commentTitle[$post['id']]));
						$this->parser->setParserVar('email', trim($commentEmail[$post['id']]));
						$this->parser->setParserVar('author_url', $commentUrl[$post['id']]);
						$this->parser->setParserVar('comment', trim($commentComment[$post['id']]));
						$this->parser->setParserVar('notify', trim($commentNotify[$post['id']]));

						$lastCommentsList .= $this->parser->parseTemplate($this->commentsTemplatePath . 'comments_form.tpl');
					} else {
						$this->parser->setParserVar('articleId', $commentPostId);
						$lastCommentsList .= $this->parser->parseTemplate($this->commentsTemplatePath . 'comments_confirm.tpl');
					}
				}

				// Post belongs Categories
				$categoryBelongsContent = array();
				
				foreach ($post['post_categories'] as $postCategory) {
					$this->parser->setMultipleParserVars($postCategory);
					$this->parser->setParserVar('categoryName', ucfirst($postCategory['category_name']));
					$categoryBelongsContent[] = $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_category_link.tpl');
				}

				if ($post['post_author']) {
					$authorData = $this->authors->getAuthor(array('authorId' => $post['post_author']));
					$this->parser->setParserVar('author_name', $authorData['author_name']);
					$this->parser->setParserVar('author_prename', $authorData['author_prename']);
					$this->parser->setParserVar('authorId', $authorData['id']);
					$this->parser->setParserVar('authorLinkName', Contentomat::makeNameWebsave($authorData['author_prename'] . ' ' . $authorData['author_name']));
				}
				
				$this->parser->setParserVar('search', $this->searchTextRequest);
				$this->parser->setParserVar('searchIn', $this->searchIn);
				$this->parser->setParserVar('categoryId', $this->categoryId);
				$this->parser->setMultipleParserVars($post);
				$this->parser->setMultipleParserVars($post['hasMedia']);
				$this->parser->setMultipleParserVars($this->mlog->paging);
				$this->parser->setParserVar('categoryBelongsContent', join(" / ", $categoryBelongsContent));
				$this->parser->setParserVar('lastCommentsList', $lastCommentsList);
				$contentList .= $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_article_row.tpl');
			}
		}

		$this->parser->setParserVar('pagingContent', $this->createPaging());
		$this->parser->setParserVar('contentList', $contentList);
		$this->content = $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_article_frame.tpl');
	}

	protected function createPaging() {
		$pagingContent = '';
		$paging = new Paging();
		$pagingData = $paging->makePaging(array(
			'pagingLinks' => $this->pagingLinks,
			'entriesPerPage' => $this->entriesPerPage,
			'totalEntries' => $this->posts->getTotalPosts(),
			'currentPage' => $this->currentPage
				));
		$pagingContent = '';
		$pagingTemplate = file_get_contents($this->mlogTemplatePath . 'mlog_paging.tpl');
		$pagingTemplate = explode("{SPLITTEMPLATEHERE}", $pagingTemplate);

		if ($this->categoryData && !empty($this->categoryData)) {
			$this->parser->setParserVar('pageId', $this->pageId);
			$this->parser->setParserVar('categoryId', $this->categoryData['id']);
			$this->parser->setParserVar('categoryName', ucfirst($this->categoryData['category_name']));
		} else {
			$this->parser->setParserVar('pageId', 10);
			$this->parser->setParserVar('categoryId', '0');
			$this->parser->setParserVar('categoryName', '');
		}

		if ($pagingData['prevPage']) {
			$this->parser->setParserVar('prevPage', $pagingData['prevPage']);
			$pagingContent .= $this->parser->parse($pagingTemplate[0]);
		}

		foreach ($pagingData['pages'] as $page) {
			$this->parser->setParserVar('listItem', $page);

			$this->parser->setParserVar('selected', $page == $this->currentPage);
			$pagingContent .= $this->parser->parse($pagingTemplate[1]);
		}

		if ($pagingData['nextPage']) {
			$this->parser->setParserVar('nextPage', $pagingData['nextPage']);
			$pagingContent .= $this->parser->parse($pagingTemplate[2]);
		}
		
		return $pagingContent;
	}

	protected function actionShowPostDetails() {
		$actionContent = '';

		// show article details
		$post = $this->posts->getPost(array('postID' => $this->postId));
		
		
		
		define('POSTTITLE', htmlspecialchars($post['post_title']));
		
		if($this->categoryData){
			define('POSTSCATEGORY',$this->categoryData['category_title_de'].' - ');
		}
		
		// update Article's total views status
		$this->posts->updatePostStatus($this->postId);

		// Article Details Ad
		$detailsInnerAd = $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_article_details_ad.tpl');



		$post['post_text'] = preg_replace('/^(.*?)\<\/p>/', '$1</p>' . $detailsInnerAd, $post['post_text']);

		// JH: Kategoriedaten auslesen!
		$this->categoryData = $this->posts->getCategoryData($this->categoryId);

		$this->parser->setMultipleParserVars($post['hasMedia']);

		$mediaContent = array();


		if ($post['postMedia']) {
			foreach ($post['postMedia'] as $mediaType => $mediaItems) {

				foreach ($mediaItems as $media) {
					$this->parser->setParserVar('mediaType', strtolower($mediaType));
					$this->parser->setMultipleParserVars($media);
					$mediaContent[$mediaType][] = $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_article_media.tpl');
				}
			}
		}

		if ($mediaContent) {
			foreach ($mediaContent as $mediaType => $mediaContent) {
				$this->parser->setParserVar('contentList' . $mediaType, join("\n", $mediaContent));
			}
		}


		switch ($post['post_options']) {
			case '1':
				$this->parser->setParserVar('mediaBarTop', 1);
				break;
			case '2':
				$this->parser->setParserVar('mediaBarBottom', 1);
				break;
			case '3':
				$this->parser->setParserVar('mediaBarTop', 1);
				$this->parser->setParserVar('mediaBarBottom', 1);
				break;
			default:
				break;
		}

		// Post belongs Categories
		$categoryBelongsContent = array();

		
		foreach ($post['post_categories'] as $postCategory) {
			$this->parser->setMultipleParserVars($postCategory);
			$this->parser->setParserVar('categoryName', ucfirst($postCategory['category_name']));
			$categoryBelongsContent[] = $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_category_link.tpl');
		}

		$this->parser->setParserVar('categoryBelongsContent', join(" / ", $categoryBelongsContent));

		if ($this->categoryData && !empty($this->categoryData)) {
			$this->parser->setParserVar('categoryId', $this->categoryData['id']);
			$this->parser->setParserVar('categoryInternalName', $this->categoryData['category_name']);
			$this->parser->setParserVar('categoryName', $this->categoryData['category_title_de']);
		} else {
			$this->parser->setParserVar('categoryId', 0);
			$this->parser->setParserVar('categoryInternalName', '');
			$this->parser->setParserVar('categoryName', '');
		}

		if ($post['post_author']) {
			$authorData = $this->authors->getAuthor(array('authorId' => $post['post_author']));
			$this->parser->setParserVar('author_name', $authorData['author_name']);
			$this->parser->setParserVar('author_prename', $authorData['author_prename']);
			$this->parser->setParserVar('authorId', $authorData['id']);
			$this->parser->setParserVar('authorLinkName', Contentomat::makeNameWebsave($authorData['author_prename'] . ' ' . $authorData['author_name']));
		}
		
		
		// Post Relations
		$relationListContent = '';
		if (is_array($this->postRelations) && !empty($this->postRelations)) {
			foreach ($this->postRelations as $relPost) {
				$this->parser->setMultipleParserVars($relPost);
				$this->parser->setParserVar('linkTitle',  Contentomat::makeNameWebsave($relPost['post_title']));
				$relationListContent .= $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_relations_row.tpl');
			}

			if($relationListContent){
				$this->parser->setParserVar('relationListContent',$relationListContent);
				$furtherInformation = $this->parser->parseTemplate($this->mlogTemplatePath.'mlog_relations_frame.tpl');
			}
			
		}

		$this->parser->setMultipleParserVars($post);

		$this->parser->setParserVar('furtherInformation', $furtherInformation);
		$this->parser->setParserVar('currentPage', $this->currentPage);
		$this->parser->setParserVar('search', $this->searchTextRequest);
		$this->parser->setParserVar('searchIn', $this->searchIn);

		// Parse Details Template
		$actionContent .= $this->parser->parseTemplate($this->mlogTemplatePath . 'mlog_article_details.tpl');



		// show comments
		if ($this->comments->commentable && $post['post_comment_status'] == '1') {

			
			$actionContent .= $this->comments->showCommentsList();

			if ($this->errorMessage[$post['id']]) {
				$actionContent .= $this->errorMessage[$post['id']];
			}
			
			if ($this->errorMessage['commentMessage']) {
				$actionContent .= $this->errorMessage['commentMessage'];
			}
			

			if ($this->showNewCommentForm) {
				$this->parser->setMultipleParserVars($this->commentVars);
				$this->parser->setParserVar('articleId', $post['id']);
				$actionContent .= $this->parser->parseTemplate($this->commentsTemplatePath . 'comments_form.tpl');
			} else {
				$this->parser->setParserVar('articleId', $commentPostId);
				$actionContent .= $this->parser->parseTemplate($this->commentsTemplatePath . 'comments_confirm.tpl');
			}
		}

		$this->content = $actionContent;
	}

}

$controller = new MlogController();
$content .= $controller->work();
?>

