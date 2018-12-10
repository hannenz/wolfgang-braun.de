<?php
/**
 * class_article.inc
 * Zeigt eine Artikelliste oder einen Artikel an
 *
 * Langkommentar
 *
 * @version 2010-03-26
 * @author A.Alkaissi <info@buero-hahn.de>
 *
 * Class Sections:
 *      1- Globale Declarations
 *      2- Getters And Setters
 *      3- Main
 *      4- Article List
 *      5- Article Details
 *      6- Paging Bar
 *      7- Services Bar
 *      8- General Functions
 */
 
 class Article {
    /****************************************************
    * Globale Declarations
    ****************************************************/
    protected $db;							// Datenbankobjekt
    protected $lastQuery;                      // Last Query
    protected $parser;						// Parserobjekt
    protected $pagingVars;                  // Paging Variables
    protected $viewVars;                    // Show Vars, Templatepfade und Parser vars.
    protected $date;                        // Aktuelle datum-zeit
    protected $pageId;                      // current page id
    protected $condOrderBy;					// ORDER BY-Teil der Artikelabfrage-Query
	protected $condWhere;					// zusaetzlicher WHERE-Teil der Artikelabfrage-Query
	protected $searchText;					// Such Text
	protected $searchMode;					// Flag, ob Suche vorhanden oder nicht
	protected $searchModeType;				// Art der Suche: 'fulltext' oder 'like'
    protected $contentTables;				// Name der Tabelle, die abgefragt werden soll
    protected $contentFieldTypes;			// Klassenvariable: Speichert MySQL-Feldtypen der aktuellen Tabelle
    protected $currentArticleData;			// Klassenvariable: Cachet den Inhalt des aktuell gelesenen Datensatzes
    protected $titleFieldPriority;          // fields priority to get title comma seperated
    protected $teaserFieldPriority;         // fields priority to get teaser comma seperated
    protected $textFieldPriority;           // fields priority to get text comma seperated
    protected $commentsTable;               // comments table name
    public $specialFormatField;				// Sonderformatierung: Feld das bearbeitet werden soll
	public $specialFormatType;				// Sonderformatierung: Formatierungsart
	public $specialFormatSearch;			// Sonderformatierung: Suchbegriff
	public $specialFormatReplace;			// Sonderformatierung: Ersetzung
        
    /****************************************************
    * Getters And Setters
    ****************************************************/
    /**
    * public function setPagingVar()
    * paging variables setter
    * @param $name string
    * @param $value string
    * @return void
    **/
    public function setPagingVar($name,$value){
        $this->pagingVars[$name] = $value;
    }
    
    /**
    * public function getPagingVar()
    * paging variables getter
    * @param $name string
    * @return array element value
    **/  
    public function getPagingVar($name){
        return $this->pagingVars[$name];
    }

    /**
    * public function setViewVar()
    * View variables setter
    * @param $name string
    * @param $value string
    * @return void
    **/
    public function setViewVar($name,$value){
        $this->viewVars[$name]=$value;
    }
    
    /**
    * public function getViewVar()
    * View variables getter
    * @param $name string
    * @return array element value
    **/  
    public function getViewVar($name){
        return $this->viewVars[$name];
    }    

	/**
	 * public function setParserVar()
	 * Setter: 
	 * @param $varName string Der gewuenschte Variablenname
     * @param $varValue string Der Variablewert
	 * @return void
	 */
	public function setParserVar($varName, $varValue) {
		$this->parser->setParserVar($varName, $varValue);
	}
    
	/**
	 * public function getParserVar()
	 * Getter: Liefert den Wert einer Parser-Variablen zurueck
	 * @param $name string Der gewuenschte Variablenname
	 * @return $r string Der Wert der Parser-Variablen
	 */
	public function getParserVar($name='') {
		$r = NULL;
		if (isset($this->parser->vars[$name])) {
			$r = $this->parser->vars[$name];
		}
		return $r;
	} 
    
	/**
	 * public function getSearchParams()
	 * @param 
	 * @return string
	 */
	public function getSearchParams(){
		$params='';
		foreach($_GET as $param=>$value){
			$noSearchParams = array("cp","pid","lang","aid","action");
			if (!$value || in_array($param, $noSearchParams)) continue;
			$params .= "&".$param."=".$value;
		}
		return $params;
	}
    
     /**
	 * public function getSearchParamsForPrintLink()
	 * Erzeugt den Querystring fÃ¼r den Druckansichtlink jeder Seite und definiert ihn als Konstante PRINTLINKPARAMS
	 *
	 * @param void
	 * @return string Parameterstring oder Leer-String
	 */
	public function getSearchParamsForPrintLink() {
		if (defined('PRINTLINKPARAMS')) return;		
		$params='';
		foreach($_GET as $param=>$value){
			$noSearchParams = array('lang','pid');
			if (!$value || in_array($param, $noSearchParams)) continue;
			$params .= '&amp;'.$param."=".$value;
		}
		define('PRINTLINKPARAMS', $params);
		return $params;
	}
    
    /**
    * protected function getArticlescommentsTotal
    * get the comments summe of an article by its id
    * @param $aid, article id
    * @return int, comments summe
    **/
    protected function getArticlescommentsTotal($aid){
       if(!PAGEID || !$aid || !$this->commentsTable){
            return false;
       }
        $query = "SELECT count(*) as commentsTotal FROM ".$this->commentsTable." WHERE comment_pid='".PAGEID."' AND comment_approved='1' AND comment_aid='".$aid."' ORDER BY comment_date  ASC";
        $this->db->query($query);
        if($r = $this->db->get(MYSQL_ASSOC)){
            return $r['commentsTotal'];
        }
        return false;
    }
    
    public function getLastQuery(){
        return $this->lastQuery;
    }
    
    /****************************************************
    * Main
    ****************************************************/
    /**
     * public function __construct()
     * Konstruktor
     * @return void
	 */
     public function __construct() {
     	// Objekte
		$this->db = new DBCex();
		$this->parser = new Parser();
        $this->pagingVars = array();
        $this->viewVars = array();
        $this->getSearchParamsForPrintLink();
     }
     
     /**
     * public function init()
     * @param $params array
     * @return void
     **/
     public function init($params=array()){
        if (!is_array($params)) {
			$params = array();
		}
        
        // Merge default Settings with Instant params
        $params = array_merge ($this->defaultParamsArray(), $params);
        
        // set articleVars
        if (is_array($params['contentTable'])){
            $this->contentTables = $params['contentTable'];
		}
        else{
            $this->contentTables = array();
        }
        if ($params['condWhere']){
            $params['condWhere'] = ' AND '.$params['condWhere'];
        }
        $this->condWhere = $params['condWhere'];
        $this->condOrderBy = $params['condOrderBy'];
        $this->searchMode =  $params['searchMode'];
        $this->searchModeType =  $params['searchModeType'];
        $this->searchText = $params['searchText'];
        $this->date = date('Y-m-d H:i:s');
        $this->commentsTable = $params['commentsTable'];

        $this->specialFormatField= $params['specialFormatField'];
        $this->specialFormatType= $params['specialFormatType'];
        $this->specialFormatSearch= $params['specialFormatSearch'];
        $this->specialFormatReplace= $params['specialFormatReplace'];
        
        $this->titleFieldPriority= $params['titleFieldPriority']; 
        $this->teaserFieldPriority= $params['teaserFieldPriority'];
        $this->textFieldPriority= $params['textFieldPriority']; 
        
        // set viewVars
        $this->setViewVar('templatePath',$params['templatePath']);                              // Allgemeiner Pfad zu den Templates der Rubrik
        $this->setViewVar('templatePaging',$params['templatePaging']);                              // Allgemeiner Pfad zu den Templates der Rubrik
        $this->setViewVar('templateRow',$params['templateRow']);                                // Template-Dateiname des Reihentemplates in der Listenuebersicht
        $this->setViewVar('templateFrame',$params['templateFrame']);                            // Template-Dateiname des Rahmentemplates in der Listenuebersicht
        $this->setViewVar('templateDetails',$params['templateDetails']);                        // Template-Dateiname der Detailansicht
        $this->setViewVar('templateArticleService',$params['templateArticleService']);          // Template-Dateiname fuer die Servicelinks unter dem Artikel in der Detailansicht
        $this->setViewVar('templateFullTextSearch',$params['templateFullTextSearch']);          // Template-Dateiname der Volltext Suchen
        $this->setViewVar('breadCrumbTitleMaxChars',$params['breadCrumbTitleMaxChars']);        // Max. ZeichenlÃ¤nge des Breadcrumbtitels
        $this->setViewVar('showArticleServicesBar',$params['showArticleServicesBar']);          // Flag: Soll Servicezeile unter Artikel angezeigt werden?
        $this->setViewVar('showSearchBoxInDetailView',$params['showSearchBoxInDetailView']);    // ...
        $this->setViewVar('getLinkTitleFromField',$params['getLinkTitleFromField']);            // Link fuer Detailansicht eines Artikels soll aus diesem DB-Feldinhalten generiert werden
        $this->setViewVar('getBreadcrumbTitleFromField',$params['getBreadcrumbTitleFromField']);// Aus diesem Feld wird der Titel der Detailseite fÃ¼r die Breadcrumbnavi erzeugt
        $this->setViewVar('specialFirstArticle',$params['specialFirstArticle']);                // specialFirstArticle      
        $this->setViewVar('specialTemplateRow',$params['specialTemplateRow']);                  // Template-Dateiname des Reihentemplates in der Listenuebersicht
        $this->setViewVar('initalLetterForArticles',$params['initalLetterForArticles']);        // Extract inital letter from text, and modefied texts (without first charachter)
        $this->setViewVar('shortTeaserLength',$params['shortTeaserLength']);        // Extract inital letter from text, and modefied texts (without first charachter)
        $this->setViewVar('longTeaserLength',$params['longTeaserLength']);        // Extract inital letter from text, and modefied texts (without first charachter)
        
                // set pagingVars
        if(intval($params['currentPage']<1)){ // no previous page for first page 
            $params['currentPage'] = 1;
        }
        $this->setPagingVar('currentPage',$params['currentPage']);                   // Nummer der aktuellen Seite
        $this->setPagingVar('currentPageStartEntrie',intval($this->pagingVars['currentPage'])-1);   // Current Page start entrie to limit query results       
        $this->setPagingVar('entriesPerPage',$params['entriesPerPage']);             // Anzahl der in der Uebersicht auf einer Seite angezeigten Eintraege
        $this->setPagingVar('pagingShowLinks',$params['pagingShowLinks']);           // Anzahl der angezeigten Auswahllinks in der Pagingreihe
        $this->setPagingVar('pagingLinkText',$params['pagingLinkText']);             // Link-Prefix fuer Auswahllinks in der Pagingreihe
        $this->setPagingVar('pagingPrevText',$params['pagingPrevText']);             // Text: Paging, eins zurueck
        $this->setPagingVar('pagingNextText',$params['pagingNextText']);             // Text: Paging, eine Seite vor
        $this->setPagingVar('entriesTotal',$params['entriesTotal']);                 // Gesamtzahl der gefundenen Artikel
        $this->setPagingVar('pagingLinkSeparator',$params['pagingLinkSeparator']);   // Text: Zeichenfolge zwischen den Paging-Elementen
        $this->setPagingVar('pagingPosition',$params['pagingPosition']);               // Paging Einblinden - Standard Wert = both (top + bottom), top, bottom, ''(leer:keine)
     }
    
    /**
    * protected function getDefaultParams()
    * @return array, default settings
    **/
    protected function defaultParamsArray(){
        // Default Einstellungen
		$defaultParams = array (
            'condWhere' => '',
            'condOrderBy' => 'online_date DESC',
            'getLinkTitleFromField' => 'title',
            'getBreadcrumbTitleFromField' => '',
            'breadCrumbTitleMaxChars' => 50,
            'showArticleServicesBar' => true,
            'templateArticleServices' => 'templates/general/article/article_service.tpl',
            'currentPage' => 1,
            'pagingPosition' => 'both',
            'entriesPerPage' => 10,
            'pagingShowLinks' => 10,
            'pagingLinkText' => 'Uebersicht',
            'pagingPrevText' => '&laquo; zur&uuml;ck',
            'pagingNextText' => 'weiter &raquo;',
            'pagingLinkSeparator' => '<span class="pagingSeparator">|</span>',
            'titleFieldPriority'=>'title',
            'teaserFieldPriority'=>'teaser,subtitle',
            'initalLetterForArticles'=>false,
            'shortTeaserLength'=>30,
            'longTeaserLength'=>100,
            'textFieldPriority'=>'text,inhalt,artikel'
		);
        return $defaultParams;
    }

    /****************************************************
    * Article List
    ****************************************************/
    /**
    * public function showArticleList()
    * @param $articleList array, article list data
    * @param $contentList string, parsed list rows as html content
    * @return string, html content
    **/
    public function showArticleList($articleList=NULL,$contentList=NULL) {
        //show Article list content
        if(!$contentList){
            $this->parser->setParserVar('contentList', $this->showArticleListRows($articleList));
        }
        else{
            $this->parser->setParserVar('contentList', $contentList);
        }
        
        // total matched articles
        $this->parser->setParserVar('entriesTotal', $this->pagingVars['entriesTotal']);
        
        // "Zeige von ... bis ..."-Anzeige
        $entriesShowTo = $this->pagingVars['entriesPerPage'] * $this->pagingVars['currentPage'];
        if ($entriesShowTo > $this->pagingVars['entriesTotal']) {
            $entriesShowTo = $this->pagingVars['entriesTotal'];
        }
        $entriesShowFrom = $this->pagingVars['entriesPerPage'] * ($this->pagingVars['currentPage'] - 1) + 1;
        $this->parser->setParserVar('entriesShowFrom', $entriesShowFrom);
        $this->parser->setParserVar('entriesShowTo', $entriesShowTo);
            
        // show top paging
        if($this->pagingVars['pagingPosition']=='top' || $this->pagingVars['pagingPosition']=='both'){
            $this->parser->setParserVar('PAGING_TOP', true);
            $showPaging = 1;
        }
        // show bottom paging
        if($this->pagingVars['pagingPosition']=='bottom' || $this->pagingVars['pagingPosition']=='both'){
            $this->parser->setParserVar('PAGING_BOTTOM', true);
            $showPaging = 1;
        }
        
        // create paging bar parser variables, if paging is enabled
        if($showPaging){
            $this->parser->setMultipleParserVars($this->createPaging());
        }
        
        // show search box if search template given
        if ($this->viewVars['templateFullTextSearch']){
			$fullTextSearch = $this->parser->parseTemplate($this->viewVars['templatePath'].$this->viewVars['templateFullTextSearch']);
			$this->parser->setParserVar('fullTextSearch', $fullTextSearch);
		}
        
        // parse frame template
        $content = $this->parser->parseTemplate($this->viewVars['templatePath'].$this->viewVars['templateFrame']);
        
        return $content;
    }
    
    /**
    * public function showArticleListRows()
    * @param $articleList array, article list data
    * @return string, parsed article rows as html string
    **/
    public function showArticleListRows($articleList=NULL) {
        // get article list data if not given
        if(!is_array($articleList)){
            $articleList = $this->getArticleList();
        }
        
        $this->parser->setParserVar("searchParameters", $this->getSearchParams());
        $this->parser->setParserVar('currentPage', $this->pagingVars['currentPage']);
        
        $rowCounter = 0;
        $contentList = '';
        foreach($articleList as $article){
            // parse special list row if enabled
            if ($this->viewVars['specialFirstArticle'] && ($this->pagingVars['currentPage'] == 1 || !$this->pagingVars['currentPage'])) {
                $this->parser->setMultipleParserVars($article);
                $contentList .= $this->parser->parseTemplate($this->viewVars['templatePath'].$this->viewVars['specialTemplateRow']);
                $this->viewVars['specialFirstArticle'] = 0;
                continue;					
            }
            // parse normal list row
            $this->parser->setParserVar('alternationFlag', $rowCounter++%2);
            $this->parser->setParserVar('rowCounter', $rowCounter);
            
            if($this->commentsTable){
                $article['commentsTotal'] = $this->getArticlescommentsTotal($article['id']);
            }
             
            $this->parser->setMultipleParserVars($article);
            
            $contentList .= $this->parser->parseTemplate($this->viewVars['templatePath'].$this->viewVars['templateRow']);
        }
        return $contentList;
    }
     
    /**
    * public function getArticleList()
    * @param $query string, sql query string
    * @return array, article list data
    **/
    public function getArticleList($query = NULL){
        // crate sql query if not given
        if (!$query){
            $query = $this->getArticlesListQuery();
        }
		
	
        $list = array();
        $this->lastQuery = $query;
        if($query){
            $this->db->query($query);
            
            // get tables fields type of query results
            $inf = $this->db->getCurrentFieldInfo();
            $this->contentFieldTypes = $inf['type'];
            while ($r = $this->db->get(MYSQL_ASSOC)) {

            	// reformat result field values
                $r = $this->formatFieldTypes($r);
                $list[] = $r;
            }
        }
        return $list;
    }
    
    /**
    * public function getArticlesListQuery()
    * create sql query of article list or article search request 
    * @return string, search query
    **/
    public function getArticlesListQuery(){
        // get single table parameters (default)
        if(!$this->contentTables){
            return false;
        }
    
        // Volltextsuche oder Like-Suche
        if ($this->searchModeType == 'fulltext') {
        	$likeMode = false;
        } else {
        	$likeMode = true;
        }
        
        // create query of "show article list"
		if (count($this->contentTables)==1){
			// Calculate total entries
			$query = $this->getTableQuery($countOnly=true,$likeMode);
			$this->db->query($query);
			$r = $this->db->get(MYSQL_ASSOC);
            $this->pagingVars['entriesTotal']= $r['entriesTotal'];
            
            // Create query
			$query = $this->getTableQuery($countOnly=false,$likeMode);
			return $query;
		}

		// create query of "search multi tables" of "search the whole Homepage"
		elseif ($this->searchMode){
			// Calculate total entries
			$query = $this->getTableQuery($countOnly=true,$likeMode);

			$this->db->query($query);
			$this->pagingVars['entriesTotal'] = 0;
			while ($r = $this->db->get(MYSQL_ASSOC)){
                $this->pagingVars['entriesTotal'] += $r['entriesTotal'];
			}
			// Create query
            $query = $this->getTableQuery($countOnly=false,$likeMode);

            return $query;
		}
		return false;
    }

    /**
    * public function getTableQuery()
    * @param $countOnly bool, true= return count query only, false=return data query 
    * @param $likemode bool, true= search table\s in like mode, false=search table\s in fulltext mode
    **/
    public function getTableQuery($countOnly=false,$likemode=false){
        
    	$tablesArray=array();
		
        foreach ($this->contentTables as $table){
			// Table parameters
            $fulltext = $table['fulltext']; // search in these field (comma seperated) in fulltext search mode
			$tablename= $table['name']; // tablename
			$relatedTables = $table['relatedTables']; // to include in query after 'FROM' keyword
			$whereConditions = $table['whereConditions'];
            $fieldMapper = $table['fieldMapper'];
            
            $status = $table['statusField']; // empty(use 'status'), none (don't test status), field name(use field name)
			if (isset($table['statusActive'])) {
				$statusActive = $table['statusActive'];
			} else {
				$statusActive = '3';
			}
			
            if(array_key_exists('dateLimit',$table)){
                $dateLimit= $table['dateLimit'];
            }
            
            // start query string
			$queryString = " (SELECT ";
            
			// query is to get articles count only
            if ($countOnly){
				$queryString .="COUNT(*) AS entriesTotal ";
            }
            
            // query is to get articles data
            else{
				$fieldToGet= array();
				if (is_array($fieldMapper)){
					foreach($fieldMapper as $key=>$value){
						$fieldToGet[]= $key . " AS ".$value;
					}
					$queryString .=  join(', ',$fieldToGet);
				}
				else
				{
					$queryString .= $tablename.".* ";
				}
            }
            
            // add standard table and other related tables after FROM keyword
            $queryString .=" FROM ".$tablename.$relatedTables;
			
            // Start query string Fragments +++++++++++++++++++++++++++++++++++++++++++++
            $queryStringFragment = array();// query items that joined with AND
            
            // test standard field (status)
            if(!$status){
                $queryStringFragment[] = $tablename . ".status = '". $this->db->dbQuote($statusActive) ."'";
            }
            // no status tested
            elseif($status == 'none'){
            }
            // use table param (status) as (status) field
            else{
                $queryStringFragment[] = $tablename . "." . $status . " = '". $this->db->dbQuote($statusActive) ."'";
            }
          
            // search a word in like '%%' mode
			if ($this->searchText && $likemode && $fulltext)
			{
				$searchFields = explode (',', $fulltext);
				$fieldsArray = array();
				foreach ($searchFields as $field){
					$fieldsArray[] = $tablename.'.'.$field." LIKE '%".$this->db->mysqlQuote($this->searchText)."%'";
				}
				if ($fieldsArray) $queryStringFragment[] = "(".join(' OR ',$fieldsArray).")";

			}
            // search a word in Fulltext mode
			elseif ($this->searchText && !$likemode && $fulltext) {
				$queryStringFragment[] ="MATCH (".$fulltext.") AGAINST ('".$this->db->mysqlQuote($this->searchText)."')";
			}
            
            // add additional Where conditions as query fragments
			if (is_array($whereConditions)){
                foreach ($whereConditions as $condition) {
                    if(trim($condition)){
                        $queryStringFragment[] = $condition;
                    }
                }
            }
			// you can give another string as $dateLimit param to change searched items Date&Time range
            if (!isset($dateLimit)){
				$queryStringFragment[] ="$tablename.online_date <= '".$this->date."' ".
				"AND ($tablename.offline_date >= '".$this->date."' ".
				"OR $tablename.offline_date = '0000-00-00 00:00:00')";
			}
			elseif(trim($dateLimit)) $queryStringFragment[] = $dateLimit;
            
            // End query string Fragments +++++++++++++++++++++++++++++++++++++++++++++++++++
            
            // join query Fragments with AND
            if(!empty($queryStringFragment)){
            $queryString .= " WHERE ".join(" AND ",$queryStringFragment);
            }
            // close single table query
			$queryString .=") ";
            
            // add single table query to tables array
			$tablesArray[]=$queryString;
            unset($dateLimit);
		}
		
        // join tables query with UNION Relation
        $queryStringFinal = join('UNION',$tablesArray);
		
        // if query is not only to count items, sort it, and limit resultes
        if (!$countOnly){
			$queryStringFinal .=" ORDER BY ".$this->condOrderBy;
			$queryStringFinal .=" LIMIT ".$this->pagingVars['entriesPerPage'] * $this->pagingVars['currentPageStartEntrie'].",".$this->pagingVars['entriesPerPage'];
		}
//var_dump($queryStringFinal);
		return $queryStringFinal;
    }    
    

    /****************************************************
    * Article Details
    ****************************************************/    
    /**
    * public function showArticleDetails()
    * get html content to show article details page
    * @param $params array, articleId and other params
    * @param $articleDetails array, article details data
    **/
    public function showArticleDetails($params=array(),$articleDetails=array()) {
        // get article details data if not given
        if (!$articleDetails){
            $articleDetails = $this->getArticleDetails($params);
        }
        
        // show article service bar if enabled
        if ($this->viewVars['showArticleServicesBar']) {
			$this->parser->setParserVar('articleService', $this->showArticleService());
		}
       
        // show search box
        if ($this->viewVars['templateFullTextSearch']){
			$fullTextSearch = $this->parser->parseTemplate($this->viewVars['templatePath'].$this->viewVars['templateFullTextSearch']);
			$this->parser->setParserVar('fullTextSearch', $fullTextSearch);
		}
        $this->parser->setParserVar("searchParameters", $this->getSearchParams());
        $this->parser->setParserVar('currentPage', $this->pagingVars['currentPage']);

        $this->parser->setMultipleParserVars($articleDetails);
        
        $articleDetailsContent = $this->parser->parseTemplate($this->viewVars['templatePath'].$this->viewVars['templateDetails']);

        return $articleDetailsContent;
    }

    /**
    * public function getArticleDetails()
    * get article details data
    * @param $params array, articleId and other params
    * @return array, article details data
    **/
    public function getArticleDetails($params=array()){
        // article id
        if (!$aid = intval($params['articleID'])){
            return false;
        }
        
        // current table params
        $table=array();
        if(count($this->contentTables)==1){
            $table = $this->contentTables[key($this->contentTables)];
        }
        else{
            return false;
        }
        
        $fulltext = $table['fulltext']; // search in these field (comma seperated) in fulltext search mode
        $tablename = $table['name']; // tablename
        $relatedTables = $table['relatedTables']; // to include in query after 'FROM' keyword
        $dateLimit = $table['dateLimit'];
        $whereConditions = $table['whereConditions'];
        $fieldMapper = $table['fieldMapper'];
        
		$status = $table['statusField']; // empty(use 'status'), none (don't test status), field name(use field name)
		if (isset($table['statusActive'])) {
			$statusActive = $table['statusActive'];
		} else {
			$statusActive = '3';
		}
        
		// Müssen Felder gemappt werden?
		$mappedFields = array();
		if (is_array($table['fieldMapper'])) {
			
			foreach($table['fieldMapper'] as $key => $value){
				$mappedFields[]= $this->db->dbQuote($key) . " AS ".$this->db->dbQuote($value);
			}
			$selectedFields =  join(', ',$mappedFields);
		} else {
			$selectedFields = $tablename.".* ";
		}
		
		
        // Start query string Fragments +++++++++++++++++++++++++++++++++++++++++++++
        
        $queryStringFragment[] = "$tablename.id = '".$aid."'  ";
        
        // test standard field (status)
        if(!$status){
            $queryStringFragment[] = $tablename . ".status = '". $this->db->dbQuote($statusActive) ."'";
        }
        // no status tested
        elseif($status == 'none'){
        }
        // use table param (status) as (status) field
        else{
            $queryStringFragment[] = $tablename . "." . $status . " = '". $this->db->dbQuote($statusActive) ."'";
        }
        
        // you can give another string as $dateLimit param to change searched items Date&Time range
        if (!$dateLimit){
            $queryStringFragment[] ="$tablename.online_date <= '".$this->date."' ".
            "AND ($tablename.offline_date >= '".$this->date."' ".
            "OR $tablename.offline_date = '0000-00-00 00:00:00')";
        } elseif (trim($dateLimit) != 'none') { 
        	$queryStringFragment[] = trim($dateLimit);
        }
        
		if ($this->condWhere){
            $queryStringFragment[] = $this->condWhere;
        }
        // End query string Fragments +++++++++++++++++++++++++++++++++++++++++++++
        
        if ($params['orderBy']) {
			$orderBy = "ORDER BY ".$params['orderBy'];
		}
        
        $query = "SELECT ". $selectedFields . " FROM ".$tablename." WHERE ";
        $query .= join(" AND ",$queryStringFragment)." ".$orderBy." LIMIT 1";
//var_dump($query);   
        $this->lastQuery = $query;
        $this->db->query($query);
		$r = $this->db->get(MYSQL_ASSOC);
        $inf = $this->db->getCurrentFieldInfo();
        $this->contentFieldTypes = $inf['type'];
        $r = $this->formatFieldTypes($r);
        
        define ('META_DESCRIPTION',$r['short_teaser']);
        $this->definePageTitleForBreadcrums($r);
        $this->currentArticleData = $r;
      
        return $r;
    }
        
    /****************************************************
    * Paging Bar
    ****************************************************/   
    /**
	 * public function createPaging()
	 * get paging bar values of article list
	 * @param void
	 * @return array, pagging navigation values
	 */
	public function createPaging() {
        
        $pagingPages = ceil($this->pagingVars['entriesTotal'] / $this->pagingVars['entriesPerPage']);
		$showLinksDistance = floor($this->pagingVars['pagingShowLinks']/2);
		$linksFrom = $this->pagingVars['currentPage'] - $showLinksDistance;
		$linksTo = $this->pagingVars['currentPage'] + $showLinksDistance;
		if ($linksFrom <= 0) {
			$linksTo += abs($linksFrom) + 1;
			$linksFrom = 1;
		}
		if ($linksTo > $pagingPages) $linksTo = $pagingPages;
        $list = array();
        if (($linksTo-$linksFrom)>1){
            for ($i = $linksFrom; $i <= $linksTo; $i++) {
                $list[] = $i;
            }
        }
        if ($this->pagingVars['currentPage'] < $pagingPages) {
            $nextPage = $this->pagingVars['currentPage'] + 1;
        }
        
        if ($this->pagingVars['currentPage'] > 1) {
            $prevPage = $this->pagingVars['currentPage'] - 1;
        }
        
        // save paging elements data
        $this->setPagingVar('data',array(
                    "linksFrom"=>$linksFrom,
                    "linksTo"=> $linksTo,
                    "nextPage" => $pagingNext,
                    "prevPage"=> $pagingPrev,
                    "pagingList"=> $list,
        ));
        
        
        // create html content
        $paging = array ();
        if ($this->viewVars['templatePaging'] && $list){
            $templatesAll = file_get_contents($this->viewVars['templatePath'].$this->viewVars['templatePaging']);
            $templateParts = explode('{SPLITTEMPLATEHERE}', $templatesAll);
            $prevTemplate = $templateParts[0];
            $listItemTemplate = $templateParts[1];
            $nextTemplate = $templateParts[2];
        
            // Pageing erzeugen
            $paging['pagingList'] =array();
            
            $this->parser->setParserVar('uri', $this->getSearchParams());
            
            //create paging list items
            foreach($list as $litsItem){
                $this->parser->setParserVar('listItem', $litsItem);
                $this->parser->setParserVar('selected', $litsItem==$this->pagingVars['currentPage']);
                $paging['pagingList'][$litsItem] = $this->parser->parse($listItemTemplate);
            }
            $paging['pagingList'] = implode($this->pagingVars['pagingLinkSeparator'], $paging['pagingList']);
            
            // create link to previous page
            if ($prevPage){
                $this->parser->setParserVar('prevPage', $prevPage);
                $paging['pagingPrev'] = $this->parser->parse($prevTemplate);
            }
            
            // create link to next page
            if ($nextPage){
                $this->parser->setParserVar('nextPage', $nextPage);
                $paging['pagingNext'] = $this->parser->parse($nextTemplate);
            }
        }

		return $paging;
	}
    

    /****************************************************
    * Services Bar
    ****************************************************/   
    /**
	 * public function showArticleService()
	 * create service toolbar for article details view
	 * @param void
	 * @return string Gibt Artikel-Servicelinks als geparstes HTML zurueck
	 */
	public function showArticleService() {
		$this->parser->setMultipleParserVars($this->currentArticleData);
		$this->parser->setParserVar('currentPage', $this->paging['currentPage']);
		$this->parser->setParserVar("searchParameters", $this->getSearchParams());
		return $this->parser->parseTemplate($this->viewVars['templateArticleService']);
	}
    
    /****************************************************
    * General Functions
    ****************************************************/ 
    /**
    * protected function formatFieldTypes()
    * @param $r array, single article data
    * @return array, modefied article data
    **/
    protected function formatFieldTypes($r){
        foreach ($r as $fieldName => $fieldValue) {
			switch ($this->contentFieldTypes[$fieldName]) {
				case 'datetime':
				case 'date':
				case 'time':
					$r = array_merge($r, $this->formatDate($fieldValue, $fieldName.'_'));
					break;

				case 'blob':
				case 'text':
					$r[$fieldName] = stripslashes($fieldValue);
					break;
					
				case 'string':
					$r[$fieldName] = stripslashes($fieldValue);
					break;
			}
		}
        return $this->extendedFormatFieldTypes($r);
    }
    
    /**
    * protected function extendedFormatFieldTypes()
    * @param $r array, single article data
    * @return array, modefied article data
    **/
    protected function extendedFormatFieldTypes($r){
        // main_title
        foreach(explode(",",$this->titleFieldPriority) as $field){
            if($r[$field]){
                $r['main_title'] = $r[$field];
                break;
            }
        }
        
        // main_text
        foreach(explode(",",$this->textFieldPriority) as $field){
            if($r[$field]){
                $r['main_text'] = $r[$field];
                break;
            }
        }
        
        // short_teaser
        foreach(explode(",",$this->teaserFieldPriority) as $field){
            if($r[$field]){
                $r['short_teaser'] = $r[$field];
                break;
            }
        }
        if (!$r['short_teaser']) {
            $r['short_teaser'] = $r['main_text'];
        } 
        $r['short_teaser'] = $this->word_split($r['short_teaser'],$this->viewVars['shortTeaserLength']);

        
        // long_teaser
        $r['long_teaser'] = $this->word_split($r['main_text'],$this->viewVars['longTeaserLength'],0,'');
        
        // link title
        $r['linkTitle'] = $this->makeNameWebSave($r[$this->viewVars['getLinkTitleFromField']]);
        
        // create inital letter and modefied texts if (initalLetterForArticles) is enabled
        if($this->viewVars['initalLetterForArticles']){
            // inital Letter
            $str_ord=ord(substr($r['main_text'],0,1));
            while ($str_ord == 46 || $str_ord == 32){
                $r['main_text'] = substr($r['main_text'],1);
                $str_ord=ord(substr($r['main_text'],0,1));
            }
            while (($str_ord & 0xF0) == 0xE0){
                $r['main_text'] = substr($r['main_text'],3);
                $str_ord=ord(substr($r['main_text'],0,1));
            }
            if (($str_ord & 0xE0) == 0xC0) $n=2;
            else $n = 1;
            $r['initalLetter'] = substr($r['main_text'],0,$n);
            
            // long teaser mod ( long teaser without inital letter)
            $r['long_teaser_mod'] = $this->word_split($r['long_teaser'],$this->viewVars['longTeaserLength'],0,'');
            $r['long_teaser_mod'] =  nl2br(trim(substr($r['long_teaser_mod'],$n)));
            
            // main text mod  ( main text without inital letter)
            $r['main_text_mod'] = preg_replace('/([^\'\>\"])(http:\/\/[^\s\)\(\<\>]*)(\s)?/',"$1<a href=\"$2\" target=\"_blank\">$2</a>$3",$r['main_text']);
            $r['main_text_mod'] = nl2br(trim(substr($r['main_text_mod'],$n)));
        }

		return $this->customFormatFieldTypes($r);
    }

    /**
    * protected customFormatFieldTypes()
    * @param $r array, single article data
    * @return array, modefied article data
    **/    
    protected function customFormatFieldTypes($r){
        switch($this->specialFormatType) {
			case '':
                break;
			case 'replace':
				$r[$this->specialFormatField] = preg_replace($this->specialFormatSearch, $this->specialFormatReplace, $r[$this->specialFormatField]);
				break;
        }
        return $r;
    }
    
    /**
	 * public function formatDate();
	 * @param string $datetime
	 * @return array Array mit mehreren Datumsangabe: 'date' => Datum, 'time' => Uhrzeit, 'timestamp'=>timestamp, 'weekday'=>Wochentag
	 */
	public function formatDate($datetime, $varNamePrefix='') {
		
		$r = array();
		
		//In case of date and time or only date (yyyy-mm-dd hh:mm:ss) or (yyyy-mm-dd) given
		if (preg_match('/^(\d{4})-(\d{2})-(\d{2})(\s(\d{2}):(\d{2}):(\d{2}))?$/', $datetime, $match)){ 

			$r[$varNamePrefix.'date'] = $match[3].'.'.$match[2].'.'.$match[1];
			$r[$varNamePrefix.'time'] = $match[5].'.'.$match[6];
			$r[$varNamePrefix.'hour'] = $match[1];

			$r[$varNamePrefix.'timestamp'] = mktime($match[5],$match[6],$match[7],$match[2],$match[3],$match[1]);
			//$r[$varNamePrefix.'weekday'] = date('l', $r[$varNamePrefix.'timestamp']);

			$r[$varNamePrefix.'weekday'] = strftime('%A', $r[$varNamePrefix.'timestamp']);
			$r[$varNamePrefix.'weekdayShort'] = substr($r[$varNamePrefix.'weekday'], 0, 3);
			$r[$varNamePrefix.'day'] = $match[3];

			$r[$varNamePrefix.'month'] = $match[2];
	        //$r[$varNamePrefix.'monthName'] = date('F', $r[$varNamePrefix.'timestamp']);
	        $r[$varNamePrefix.'monthName'] = strftime('%B', $r[$varNamePrefix.'timestamp']);
			//$r[$varNamePrefix.'monthNameShort'] = date('M', $r[$varNamePrefix.'timestamp']);
			$r[$varNamePrefix.'monthNameShort'] = substr($r[$varNamePrefix.'monthName'], 0, 3);

			$r[$varNamePrefix.'year'] = $match[1];
			
			if ($r[$varNamePrefix.'date'] == '..' || $r[$varNamePrefix.'date'] =='00.00.0000') {
				$r[$varNamePrefix.'date'] = '';
			}
			
			if ($r[$varNamePrefix.'time'] == '.' || $r[$varNamePrefix.'time'] =='00.00') {
				$r[$varNamePrefix.'time'] = '';
			}
			
		// In case of only time (hh:mm:ss) given
		} else if ( preg_match('/^(\d{2}):(\d{2}):(\d{2})$/', $datetime, $match)){
		
			$r[$varNamePrefix.'time'] = $match[1].'.'.$match[2];
			$r[$varNamePrefix.'hour'] = $match[1];
			
			if ($r[$varNamePrefix.'time'] == '.' || $r[$varNamePrefix.'time'] =='00.00') {
				$r[$varNamePrefix.'time'] = '';
			}
       } 

		return $r;
	}
    
    /**
    * function word_split()
    * get teaser or short strings from another text
    * @param $str string, source text
    * @param @words int, target text length in words
    * @param @prefix string, string to add to the front of target text
    * @param @srffix string, string to add to the end of target text
    * @return string, target text (teaser or short string)
    **/
	function word_split($str,$words=15,$prefix=false,$suffix=false) {
		$str = strip_tags($str);
		$arr = preg_split("/[\s]+/", $str,$words+1);
		$arr = array_slice($arr,0,$words);
		$text=join(' ',$arr);
		if ($prefix) $text = $prefix.$text;
		if ($suffix) $text .= $suffix;
		return trim($text);
	}   
    
    /**
    * public function definePageTitleForBreadcrums() 
    * defined Page title for breadcrums
    * @param array, article data
    * @return void
    **/
	public function definePageTitleForBreadcrums($r=array()) {
		if (!defined('BREADCRUMBPAGETITLE')) {
			$text = $r[$this->getViewVar('getBreadcrumbTitleFromField')];
			$postfix = '';
			
			if (strlen($text) > $this->getViewVar('breadCrumbTitleMaxChars')) {
				preg_match('/^(.{'.$this->getViewVar('breadCrumbTitleMaxChars').',}\b)/Uis', $text, $match);
				$text = $match[1];
				$postfix = ' ...';
			} 
			if ($text) {
				define ('BREADCRUMBPAGETITLE', $text.$postfix);
			}
		}		
	}
    
    /**
	 * public function makeNameWebSave()
	 * Hilfsfunktion: Tauscht alle ungueltigen/kritischen Zeichen in einem Datei-/Verzeichnisnamen/-pfad aus
	 * @param $name string Datei-/Verzeichnis-/Pfadname
	 * @return string Es wird ein korrekter Name zurueckgegeben
	 */
	public function makeNameWebsave($name) {
		
		// Siehe auch http://www.ascii-code.com/
		$substituteChars = array(
			32 => '-',	// Leerzeichen
			33 => '',	// !
			34 => '',	// "
			35 => '',	// #
			36 => '',	// $
			38 => '-',	// &
			39 => '',	// '
			40 => '-',	// (
			41 => '-',	// )
			42 => '-',	// *
			43 => '-',	// +
			47 => '-',	// /
			46 => '-',	// .
			63 => '',	// ?
			64 => '-',	// @
			95 => '-',	// _
			171 => '',	// ï¿½
			187 => '',	// ï¿½
			192 => 'A',	// A with grave (franz.)
			193 => 'A',	// A with acute (franz.)
			194 => 'A',	// A with Circumflex (franz.)
			195 => 'A',	// A mit Tilde (franz.)
			196 => 'AE',	// A mit Umlaut (dt.)
			197 => 'A',	// A mit Kreis (skand.)
			198 => 'AE',	// A mit E (franz.)
			199 => 'C', // C mit Hakenb (franz.)
			200 => 'E', // E with grave (franz.)
			201 => 'E', // E with acute (franz.)
			202 => 'E', // E with circumflex (franz.)
			203 => 'E', // E mit Doppelpunkt (franz.)
			204 => 'I', // I with grave (franz.)
			205 => 'I', // I with acute (franz.)
			206 => 'I', // I with circumflex (franz.)
			207 => 'I', // I mit Doppelpunkt (franz.)
			208 => 'D', // D mit Strich (?)
			209 => 'N', // N mit Tilde (?)
			210 => 'O', // O with grave (franz.)
			211 => 'O', // O with acute (franz.)
			212 => 'O', // O with circumflex (franz.)
			213 => 'O', // O mit Tilde (franz.)
			214 => 'OE', // O mit Umlaut (dt.)
			216 => 'O', // O durchgestrichen (skand.)
			217 => 'U', // U with grave (franz.)
			218 => 'U', // U with acute (franz.)
			219 => 'U', // U with circumflex (franz.)
			220 => 'Ue', //chr(85).chr(69) // U mit Umlaut (dt.)
			223 => 'ss', // scharfes S (dt.)
			224 => 'a', // a with grave (franz.)
			225 => 'a', // a with acute (franz.)
			226 => 'a', // a with circumflex (franz.)
			227 => 'a', // a mit Tilde (franz.)
			228 => 'ae', // a mit Umlaut (dt.)
			229 => 'a', // a mit Kreis (skand.)
			230 => 'ae', // a mit e (franz.)
			231 => 'c', // c mit Haken (franz.)
			232 => 'e', // e with grave (franz.)
			233 => 'e', // e with acute (franz.)
			234 => 'e', // e with circumflex (franz.)
			235 => 'e', // e mit Umlaut (franz.)
			236 => 'i', // i with grave (franz.)
			237 => 'i', // i with acute (franz.)
			238 => 'i', // i with circumflex (franz.)
			239 => 'i', // i mit Umlaut (franz.)
			241 => 'n', // n mit Tilde (?)
			242 => 'o', // o with grave (franz.)
			243 => 'o', // o with acute (franz.)
			244 => 'o', // o with circumflex (franz.)
			245 => 'o', // o mit Tilde (franz.)
			246 => 'oe', // o mit Umlaut (franz.)
			249 => 'u', // u with grave (franz.)
			250 => 'u', // u with acute (franz.)
			251 => 'u', // u with circumflex (franz.)
			252 => 'ue', // u mit Umlaut (dt.)
			253 => 'y', // y with acute (franz.)
			255 => 'y', // y mit Umlaut (franz.)
		);

		if (mb_detect_encoding($name) == 'UTF-8' && mb_check_encoding($name, 'UTF-8')) {
			$name = utf8_decode($name);
		}

		$newName = '';
		$nameLength = mb_strlen($name);
		
		for ($c = 0; $c < $nameLength; $c++) {
			$char = mb_substr($name, $c, 1);
			$charNumber = ord($char);

			// Zeichen zwischen ASCII 32 und 126 (normale Zeichen)
			if ($charNumber > 31 || $charNumber < 123) {
				
				// Zeichen im Array zum Wechseln vorhanden?
				if (in_array($charNumber, array_keys($substituteChars))) {
					$newChar = $substituteChars[$charNumber];
				} else {
					$newChar = $char;
				}
			// Zeichen Ã¼ber 123 (event. Sonderzeichen wie Umlaute)
			} else if (in_array($charNumber, array_keys($substituteChars))) {
					$newChar = $substituteChars[$charNumber];
			// Falls Zeichen nicht in Austausch-Array, dann entfernen
			} else {
				$newChar = '';
			}
			$newName .= $newChar;
		}
		
		return preg_replace('/-{2,}/', '-', trim($newName));
	}
 }
 ?>