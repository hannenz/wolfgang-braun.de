<?php
// TODO: das müsste namespace Contentomat/Paperboy werden!
namespace Contentomat;

/**
 * modify_html.php
 *
 * @author Abbas Al Kaissi <info@buero-hahn.de>
 * @version 2013-07-24
 */



// sections parameters
$sections = array(
	'header'=>array(
		'template'=>'newsletter_header.tpl',
		'unitsRegex'=>"/<!-- START NEWSLETTER HEADER -->(.*?)<!-- END NEWSLETTER HEADER -->/",
		'itemsRegex'=>'/<div class="newsletterHead">.*?src="(?P<image>.*?)".*?<\/div>/',
	),
	'editorial'=>array(
		'template'=>'newsletter_editorial.tpl',
		'unitsRegex'=>"/<!-- START NEWSLETTER EDITORIAL -->(.*?)<!-- END NEWSLETTER EDITORIAL -->/",
		'itemsRegex'=>'/<h1 class="newsletterArticleHead">(?P<title>.*?)<\/h1>.*<img .*?(src="(?P<image>.*?)").*?\>.*<div class="newsletterArticleText">(?P<text>.*?)<\/div>/'
	),
	'article'=>array(
		'template'=>'newsletter_article.tpl',
		'unitsRegex'=>"/<!-- START NEWSLETTER ARTICLE -->(.*?)<!-- END NEWSLETTER ARTICLE -->/",
		'itemsRegex'=>'/<h3 class="newsletterArticleHead">(?P<title>.*?)<\/h3>(?P<image>.*?)?<div class="newsletterArticleText">(?P<text>.*?)<\/div>.*?<div class="newsletterArticleLink">(?P<link>.*?)<\/div>/'
	),
	'footer'=>array(
		'template'=>'newsletter_footer.tpl',
		'unitsRegex'=>"/<!-- START NEWSLETTER FOOTER -->(.*?)<!-- END NEWSLETTER FOOTER -->/",
		'itemsRegex'=>'/(?P<footer>.*)/',
	),
	
);

$parser = new Parser();



// MAIN

// remove return line
$source = preg_replace('/\n/', '', $mailContentHtml);

// add path prefix to images sources
$source = preg_replace('/src=([\'|"]?)/', 'src=\1'.PATHTOWEBROOT, $source);

$mailContent = newsletterHTML($sections,$source);

$parser->setParserVar('mailContent', $mailContent);

// Parse Main Frame
$mailContentHtml = $parser->parseTemplate(PATHTOWEBROOT.'templates/paperboy_add/newsletter_frame.tpl');


$mailContentText = newsletterTEXT($sections,$source);



/**
 * function newsletterHTML()
 * 
 * convert frontend layout to table based layout to send it safe in e-mails
 * 
 * @param array $sections
 * @param text $source
 * @return text 
 */
function newsletterHTML($sections,$source){
	$parser = new Parser();
	$mailContentHlp = array();
	$headerContent = '';
	
	foreach($sections as $sectionName => $section){
		preg_match_all($section['unitsRegex'], $source,$units);
		if(is_array($units)){
			foreach($units[0] as $unit){
				preg_match_all($section['itemsRegex'],$unit, $items);
				foreach($items as $name => $parserVars){
					
					if(is_numeric($name)) continue;
					switch($name){
						case 'image':
							$value = preg_replace('/.*?<img.*src="(.*)".*>/',"$1",trim($parserVars[0]));
							break;
						default:
							$value = $parserVars[0];
							break;
					}
					
					$parser->setParserVar($name, $value);
				}
				
				if($sectionName=='header'){
					$headerContent = $parser->parseTemplate(PATHTOWEBROOT.'templates/paperboy_add/'.$section['template']);
				}else{
					$mailContentHlp[] = $parser->parseTemplate(PATHTOWEBROOT.'templates/paperboy_add/'.$section['template']);
				}
			}
		}
	}
	$seperator = $parser->parseTemplate(PATHTOWEBROOT.'templates/paperboy_add/newsletter_separator.tpl');
	return $headerContent.join($seperator,$mailContentHlp);
}


/**
 * function newsletterTEXT()
 * 
 * convert frontend layout to plain text e-mails
 * 
 * @param array $sections
 * @param text $source
 * @return text 
 */
function newsletterTEXT($sections,$source){
	$parser = new Parser();
	$mailContentHlp = array();
	foreach($sections as $sectionName => $section){

		
		preg_match_all($section['unitsRegex'], $source,$units);
		if(is_array($units)){
			foreach($units[0] as $unit){
				preg_match_all($section['itemsRegex'],$unit, $items);
				$unitTextContent = '';
				foreach($items as $name => $parserVars){
					if(is_numeric($name)) 		continue;
						
					switch($name){
						case 'title':
							$unitTextContent .= "*".clearText($parserVars[0])."*"."\n\n";
							break;
						case 'text':
							$unitTextContent .= clearText($parserVars[0])."\n\n";
							break;
						case 'footer':
							$text = clearText($parserVars[0])."\n\n";
							$text = preg_replace('/:\*\s*/', ":*\n\n", $text);
							$unitTextContent .= $text;
							break;
						case 'link':
							$text = clearText($parserVars[0])."\n\n";
							$text = preg_replace('/</', "\n<", $text);
							$unitTextContent .= $text ;
							break;
						default:
							break;
					}
				}
				if($unitTextContent){
					$mailContentHlp[] = $unitTextContent;
				}
			}
		}
	}
	$seperator = "---------------------------------------------------------------------------\n\n";
	
	return join($seperator,$mailContentHlp);
}


/**
 *
 * @param type $text
 * @return type 
 */
function clearText($text){
	$clear = preg_replace('/<br\s*\/>/', "\n", $text);
	$clear = preg_replace( '/<a.*?href="mailto:(.*?)".*?>.*?<\/a>/', "*-$1-*",$clear);
	$clear = preg_replace( '/<a.*?href="(.*?)".*?>(.*?)<\/a>/', "$2 *-$1-*",$clear);
	$clear = preg_replace('/<b>(.*?)(\s*)<\/b>/', "*$1*$2", $clear);
	$clear = preg_replace('/\t/', "", $clear);
	$clear = preg_replace('/\n\r/', "\n", $clear);
	$clear = strip_tags($clear);
	$clear = preg_replace('/&nbsp;/', ' ', $clear);
	$clear = preg_replace('/\t;/', ' ', $clear);
	$clear = html_entity_decode($clear);
	$clear = urldecode($clear);
	$clear = preg_replace('/ +/', ' ', $clear);
	$clear = preg_replace('/-\*/', '>', $clear);
	$clear = preg_replace('/\*-/', '<', $clear);
	$clear = trim($clear);
	return $clear;
}




?>
