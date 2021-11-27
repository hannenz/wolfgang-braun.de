<?php
/**
 * class Controller
 * 
 * @author A.Al Kaissi, J.Hahn <info@content-o-mat.de>
 * @version 2014-07-09
 */
namespace Contentomat;

class Controller {

	/**
	 * Global Get Variables
	 * 
	 * @var array  $getvars
	 */
	protected $getvars;

	/**
	 * Global Post Variables
	 * 
	 * @var array $postvars
	 */
	protected $postvars;

	/**
	 * Instance of Database Class
	 * 
	 * @var object $db
	 */
	protected $db;

	/**
	 * Instance of Class Parser
	 * 
	 * @var object $parser
	 */
	protected $parser;

	/**
	 * Current Content-o-mat page ID
	 * 
	 * @var string $pageId
	 */
	protected $pageId;

	/**
	 * Current Page Language shortcut
	 * 
	 * @var string $pageLang 
	 */
	protected $pageLang;

	/**
	 * Set default frontend templates path
	 * 
	 * @var string  $templatesPath 
	 */
	protected $templatesPath;

	/**
	 * Set default frontend php include path
	 * 
	 * @var string $phpIncludesPath
	 */
	protected $phpIncludesPath;

	/**
	 * Set default frontend classes path 
	 * 
	 * @var string $phpClassesPath
	 */
	protected $phpClassesPath;

	/**
	 * Current page action
	 * 
	 * @var string  $action
	 */
	protected $action;

	/**
	 * Set Default Action Name, default is default
	 * 
	 * @var string  $defaultAction
	 */
	protected $defaultAction;

	/**
	 * Array of error messages
	 * 
	 * @var array $errorMessage 
	 */
	protected $errorMessage;

	/**
	 * Bool Flag, to output content in ajax format
	 * 
	 * @var bool $isAjax 
	 */
	protected $isAjax;

	/**
	 * Bool Flag, output content as json encoded string
	 * 
	 * @var bool $isJson 
	 */
	protected $isJson;

	/**
	 * Output Content
	 * @var mixed $content
	 */
	protected $content; // 

	
	/*
	 * public function setAjax()
	 * 
	 * set the ajax flag value, if flag set to true will return content
	 * 
	 */
	public function setAjax($value){
		if(trim($value)){
			$this->isAjax = true;
		}else{
			$this->isAjax = false;	
		}
	}
	/**
	 * public function __construct()
	 */

	public function __construct($params=array()) {

		// assign name=>value parameters to global public variables in class
		if(!empty($params)){
			foreach($params as $varName => $value){
				if(!is_numeric($varName)){
					$this->$varName = $value;
				}
			}
		}

		// Global Get Variables
		$this->getvars = $_GET;

		// Global Post Variables
		$this->postvars = $_POST;

		// Instance of Database Class
		$this->db = new DBCex();

		// Instance of Class Parser
		$this->parser = new Parser();

		// Current Content-o-mat page ID
		$this->pageId = intval(trim($_GET['pid']));

		// Current Page Language shortcut
		$this->pageLang = trim($_GET['lang']);

		// Set default frontend templates path
		$this->templatesPath = PATHTOWEBROOT . 'templates/';

		// Set default frontend php include path
		$this->phpIncludesPath = PATHTOWEBROOT . 'phpincludes/';

		// Set default frontend classes path
		$this->phpClassesPath = PATHTOWEBROOT . 'phpincludes/classes/';

		// Set Default Action Name, default is default
		$this->defaultAction = 'default';

		// Bool Flag, to output content in ajax format
		$this->isAjax = false;

		// Bool Flag, output content as json encoded string
		$this->isJson = false;

		// Initialize main component
		$this->init();
		
		// Initialize actions
		$this->initActions();
	}

	/**
	 * public function init()
	 * 
	 * overrides to extend __construct methode
	 * 
	 */
	public function init() {
		
	}


	/**
	 * protected function initActions()
	 * 
	 * set action type, from any GET or POST variable named has name "action"
	 * if not then take the default action name "default" as current action name.
	 * 
	 *  usually overrides in extended controller class 
	 * 
	 * @param string $action 
	 * @retrun void
	 */
	protected function initActions($action = '') {
		
		// action is set in extending class' init() method
		if ($this->action) {
			return;
		}
		
		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			if ($this->getvars['action']) {
				$this->action = $this->getvars['action'];
			} else if ($this->postvars['action']) {
				$this->action = $this->postvars['action'];
			} else {
				$this->action = $this->defaultAction;
			}
		}
	}

	/**
	 * protected function load()
	 * load php script files using include_once
	 * @param type $path 
	 */
	protected function load($path) {
		include_once($path);
	}

	/**
	 * Public function work()
	 * 
	 * Run Page action and return/output content
	 * 
	 * @return string 
	 */
	public function work() {

		$this->doAction($this->action);

		// if mixed variable $this->content is an array, must output it as 
		// json string, to not assign an array to cmt page $content variable
		if(is_array($this->getContent())){
			$this->isJson = true;
		}
		
		// AJAX + JSON OUTPUT
		// JSON flag set also to true:
		// 1- convert output content to json format 
		// 2- print page content with echo
		// 3- and exit processing the rest of page
		if ($this->isJson) {
			echo json_encode($this->getContent());
			exit;
		}

		// AJAX OUTPUT
		// If Ajax action flag is set to true:
		// 1- print page content with echo
		// 2- and exit processing the rest of page
		elseif ($this->isAjax) {
			echo $this->getContent();
			exit;

			// RETURN STRING
			// return page content as HTML string
		} else {
			return $this->getContent();
		}
	}

	/**
	 * 	public function doAction()
	 * 
	 * Call page current action
	 * 
	 * @param string $action
	 * @return void 
	 */
	public function doAction($action) {
		if (!method_exists($this, 'action' . ucfirst($action))) {
			$action = $this->defaultAction;
		}

		$actionMethod = 'action' . ucfirst($action);
//		$this->action = $action;

		$this->$actionMethod();
	}

	/**
	 *	protected function changeAction()
	 * 
	 * redirect to action with/without reseting of resent output content
	 * @param type $action
	 * @param type $resetContent 
	 */
	protected function changeAction($action,$resetContent = true){
		if($resetContent){
			$this->content = '';
		}
		$this->doAction($action);
	}
	/**
	 * public function getContent() 
	 * 
	 * get the content of page as HTML string
	 * 
	 * @return string
	 */
	public function getContent() {
		return $this->content;
	}

	// Default Action
	/**
	 * public function actionDefault()
	 * 
	 * Default page action
	 * overrides in extended controllers  
	 * simiulate the default: case, in switch statement
	 */
	public function actionDefault() {
		//
	}
	
}

?>