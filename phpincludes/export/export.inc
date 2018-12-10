<?php

include_once (PATHTOADMIN . 'classes/class_controller.inc');

/**
 * Class ExportController .. Local Class extends Controller
 */
class ExportController extends Controller {

	private $exporter;

	public function init() {
		$this->load(PATHTOADMIN . 'classes/class_exporter.php');
		$this->load(PATHTOADMIN . 'classes/class_exporter_xml.php');
		//$this->load(PATHTOADMIN . 'classes/class_exporter_cmt.php');
	}

	protected function initActions($action = '') {

		if (trim($action) != '' && !is_array($action)) {
			$this->action = trim($action);
		} else {
			$this->action = 'dumpDB';
		}
	}

	protected function actionDumpDB() {
		
		$exportFormats = array('sql', 'csv', 'xml', 'cmt', 'tpl');
		//$exportFormats = array('cmt');
		
		foreach ($exportFormats as $format) {
			
			$this->load(PATHTOADMIN . 'classes/class_exporter_'.$format.'.php');
			
			$className = ucfirst($format) . 'Exporter';
			$exporter = new $className();
			$condWhere = array();
			//$customQuery= 'SELECT id, widget_html FROM widgets';
			$exportString = $exporter->init(array(
				
				// global settings
				'exportStructure' => true,		// default is True
				'exportData' => true,			// default is True
				'exportCmtFields' => true,		// default is True
				'exportCmtTables' => true,		// default is True
				'templateFrame' => $this->templatesPath . 'export/export_template_frame.tpl',
				'templateRow' => $this->templatesPath . 'export/export_template_row.tpl',
				
				// 1- get query
				'customQuery' => $customQuery,
				
				// 2- get tables
				'tables' => array(
					//'rvr_news'=>array(),
					//'mlog_comments' => array()
					//'widgets' => array('customQuery' => 'SELECT id, widget_html FROM widgets'),
					//'cmt_execute_code' => array()
				),
			  )
			);
			$exporter->export(array(
				'save' => true,
				'download' => false,
				'filePrefix' => 'mysql_dump_',
				//'fileExtension'=>'html'
				//'fileName'=>'myFile'
			));
		}
		
		exit;
	}

}

$controller = new ExportController();
$content .= $controller->work();
?>

