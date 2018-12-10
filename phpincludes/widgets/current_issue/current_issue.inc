<?php 
/**
 * current_issue.inc
 * Widget: Zeigt die aktuelle Heftausgabe an
 *  
 */

	$tplPath = 'templates/widgets/current_issue/';
	
	$db = new DBCex();
	$parser = new Parser();
	
	$db->query("SELECT * FROM na_issues WHERE online_date <= '" . date('Y-m-d') . "' ORDER BY online_date DESC LIMIT 1");
	$r = $db->get();
	
	$parser->setMultipleParserVars($r);
	$content = $parser->parseTemplate($tplPath . 'current_issue_frame.tpl');
?>