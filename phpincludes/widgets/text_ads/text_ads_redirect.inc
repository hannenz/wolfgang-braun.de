<?php
/**
 * text_ads_redirect.inc
 * Leitet zur entsprechenden Seite weiter, wenn eine Textanzeige geklickt wurde.
 * 
 * @version 2012-09-28
 */
	include_once (PATHTOWEBROOT . 'phpincludes/classes/class_text_ads.inc');
	
	
	$adId = intval(trim($_GET['ad']));
	$url = trim($_GET['url']);
	
	// update clicks status
	if($adId){
		$textAds = new TextAds();
		$textAds->increaseClicks($adId);
	}
	
	
	// Redirect
	$pattern = '/^(([\w]+:)?\/\/)?(([\d\w]|%[a-fA-f\d]{2,2})+(:([\d\w]|%[a-fA-f\d]{2,2})+)?@)?([\d\w][-\d\w]{0,253}[\d\w]\.)+[\w]{2,4}(:[\d]+)?(\/([-+_~.\d\w]|%[a-fA-f\d]{2,2})*)*(\?(&amp;?([-+_~.\d\w]|%[a-fA-f\d]{2,2})=?)*)?(#([-+_~.\d\w]|%[a-fA-f\d]{2,2})*)?$/';
	
	if($url && preg_match($pattern, $url)){
		header('Location: '.$url);
	}else{
		$content .= "The URL is not valid and cannot be loaded";
	}


?>