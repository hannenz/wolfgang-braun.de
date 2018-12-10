<?php
namespace Website;

use \Contentomat\Debug;
use \Contentomat\PsrAutoloader;
use \Contentomat\Controller;
use \Contentomat\Mail;

class TestController extends Controller {
	
	public function actionDefault() {
		$pages = array(
			array('cmt_title' => 'Seite 1'),
			array('cmt_title' => 'Seite 2'),
			array('cmt_title' => 'Noch ne Seite')
		);
		$this->parser->setParserVar('pages', $pages);
		$this->content = $this->parser->parsetemplate(PATHTOWEBROOT . 'templates/parsertest/parsertest.tpl');
	}
}

$autoLoad = new PsrAutoloader();
//$autoLoad->addNamespace('Contentomat', INCLUDEPATHTOADMIN . 'classes');
$autoLoad->addNamespace('Website', INCLUDEPATH . 'phpincludes/classes');

$testController = new TestController();
$content .= $testController->work();
?>