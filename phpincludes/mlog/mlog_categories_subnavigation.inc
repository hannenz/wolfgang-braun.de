<?php 
/**
 * mlog_categories_subnavigation.inc 
 * Erzeugt eine navigation aus den Rubriken des MLog
 * 
 * @version 2012-09-04
 * @author A.Alkaissi <info@buero-hahn.de>
 */
 
	include_once (PATHTOADMIN . 'classes/app_mlog/class_mlog_posts.inc');
	$templatePath = PATHTOWEBROOT.'templates/mlog/';
	
	$categoriesExclude = array('6','7');

	// Falls Seiten-ID = 10, dann ID der ausgewählten MLog-Rubrik merken
	if($_GET['pid'] == '10' && $_GET['cat']){
		$selectedID = intval($_GET['cat']);
	} else {
		$selectedID = 0;
	}
	
	$parser= new Parser();
	$mlog = new Posts();

	$categories = $mlog->getPostsCategories();
	
	$subNavigationContent = '';
	
	foreach($categories as $category){
		if(in_array($category['id'],$categoriesExclude)) continue;
		
		$category['categoryName'] = ucfirst($category['category_name']);
	    $parser->setParserVar("categorySelected",($selectedID==$category['id']));
		
	    $parser->setMultipleParserVars($category);
	    $subNavigationContent .= $parser->parseTemplate($templatePath.'mlog_category_subnavigation_element.tpl');
	}
	
	$content .= $subNavigationContent;

?>