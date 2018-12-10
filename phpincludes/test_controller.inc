<?php
namespace Foo;

use \Contentomat\Debug;
use \Contentomat\PsrAutoloader;
use \Contentomat\Controller;
use \Contentomat\Mail;

class TestController extends Controller {
	
	public function actionDefault() {

		if (!empty($_POST['email'])){
			$mail = new Mail();
			$mail->send(array(
				'senderName' => 'TestController',
				'senderEmail' => 'info@agentur-halma.de',
				'subject' => 'Default Action',
				'text' => 'This is a very simple TEXT email (no HTML parts!)',
				'recipient' => $_POST['email'],
				'transport' => 'mail'
			));
		}
		else {
			$this->content = '<p>This is the TestController\'s default action</p>';
			Debug::log('This is the TestController\'s default action');

			$this->content .= '<form action="/de/28/Testseite.html" method="post"><label for="email">Test E-Mail senden an</label><input type="email" name="email" id="email" /><input type="submit" /></form>';
		}
	}
}

$autoLoad = new PsrAutoloader();
$autoLoad->addNamespace('Contentomat', INCLUDEPATHTOADMIN . 'classes');
$autoLoad->addNamespace('Foo', INCLUDEPATH . 'phpincludes/classes');

$testController = new TestController();
$content .= $testController->work();

?>
