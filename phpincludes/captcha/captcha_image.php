<?php
/**
 * captcha.php
 * Erzeugt eine Captcha-Grafik und speichert diese als Sessionvariable
 * 
 * @authos J.Hahn <info@buero-hahn.de>
 * @version 2008-07-21
 * 
 */
	error_reporting(0);
	define ('PATHTOADMIN', '../../admin/');
	define ('PATHTOWEBROOT', '../../');
	require (PATHTOADMIN.'cmt_constants.inc');
	include (PATHTOADMIN.'cmt_functions_website.inc');
	include (PATHTOADMIN.'classes/class_session.php');
	include (PATHTOADMIN.'classes/class_image.php');
	
	$session = new Session();
	$image = new Image();


	// Code erzeugen
	$chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$charsLength = strlen($chars)-1;
	$codeLength = 6;
	$code = '';
	
	for ($i=1; $i<=$codeLength; $i++)	{
		$rand = intval(rand(0,10));
		if ($rand<5) {
			$code .= $rand;
		} else {
			$code .= substr($chars, intval(rand(0, $charsLength)), 1);
		}
	}
	
	// Code speichern
	$session->setSessionVar('mlogCaptcha', $code);
	$session->saveSessionVars();
	// Bild ausgeben
	$image->createCaptchaImage( array(	'captchaCode' => $code,
										'imageType' => 'jpg',
										'imageWidth' => 100,
										'font' => PATHTOWEBROOT.'fonts/dreamofme.gdf'
									));
	exit();
?>