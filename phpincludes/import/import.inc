<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class ImportController .. Local Class extends Controller
 */
class ImportController extends Controller {

	private $importer;
	protected $app;

	public function init() {
		$this->load(PATHTOADMIN . 'classes/class_importer.php');
		$this->load(PATHTOADMIN . 'classes/class_fieldhandler.inc');
		//$this->load(PATHTOADMIN . 'classes/class_applicationcontroller.inc');
		$this->load(PATHTOADMIN . 'classes/class_applicationhandler.inc');
		$this->app = new ApplicationHandler();
		$this->initActions();
		$this->initImporter();
	}

	protected function initImporter() {
		$this->importer = new Importer();
	}

	protected function initActions($action = '') {

		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'importDB';
		}
	}


	protected function actionImportDB() {
		/*
		$data = $this->app->createTable(
				array(
					'tableName'=>'testTable'
				)
				
				
				);
		var_dump($data);
		*/

		$this->importer->import(array(
			'fileName'=>'mysql_dump_2013-07-01_173550.cmt.xml',
			'filePath'=>INCLUDEPATH.'export/',
			'fileType'=>'cmt',			// sql, csv, cmt
			'importType'=>'update',		// update, add, truncate
			'tables'=>array('au_country','au_download_category','au_download','au_news')
		));
		
		exit;
	}

}

$controller = new ImportController();
$content .= $controller->work();
?>

