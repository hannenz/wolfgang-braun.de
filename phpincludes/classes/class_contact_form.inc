<?php
/**
 * class_contact_form.inc
 * Stellt alle Methoden zur Verfügung, um ein Kontaktformular zu erstellen.
 * 
 * @version 2011-03-10
 * @author J.Hahn <info@buero-hahn.de>
 */

Class ContactForm extends DataValidation {
	
	protected $formTemplate;
	protected $formFieldNames;
	protected $formFieldOptions;
	protected $formFieldData;
	protected $formFieldErrors;
	protected $formFieldTypes;
	protected $formFieldPrefix;
	
	public function __construct() {
		
		$this->formFieldNames = array();
		$this->formFieldOptions = array();
		$this->formFieldErrors = array();
		$this->defaultCountry = 'de';
		$this->formFieldPrefix = '';
		
		// Mögliche Sondertypen für Formularfelder. Relevant für die Überprüfung
		$this->formFieldTypes = array(
			'email',
			'zip',
			'phone',
			'number',
			'isset',
			'password'
		);
	}
	
	/**
	 * 
	 * public function parseFormFieldsInTemplate()
	 * Sucht in dem übergebenen Formulartemplate nach den Formularfeldern, die in der Mail versendet werden sollen. Die
	 * gefundenen Feldnamen werden in dem Klassenarray $formFieldNames gespeichert.
	 *
	 * @param array $params Variablen als Schlüssel/Wert-Paare
	 * - templatePath => optional: Pfad zur Templatedatei
	 * - template => optional: Inhalt (HTML) des Templates
	 *
	 * @return boolean True oder false, je nachdem, ob ein Template und Formularfelder darin gefunden wurden.
	 */
	public function parseFormFieldsInTemplate($params = array()) {

		// Falls weder Pfad zu Template noch Template selbst (string) übergeben wurde, dann fals zurückgeben
		if (!$params['templatePath'] && !$params['template']) {
			return false;
		}
	
		// Template auslesen
		if ($params['templatePath']) {
			$template = file_get_contents(trim($params['templatePath']));
		} else {
			$template = $params['template'];
		}
		
		// Prefix speichern
		$this->formFieldPrefix = trim($params['formFieldPrefix']);

		// 1. Felder auslesen
		$regExp = '/(<input|<textarea|<select)(.*)>/Uis';
		preg_match_all($regExp, $template, $match);

		// 2. Feldnamen und -anweisungen auslesen
		$regExpName = '/name=(\'|")?('.preg_quote($this->formFieldPrefix, '/').'[-_a-z0-9]+?)(\'|")?/Uis';
		$regExpTitle = '/title=(\'|")?\s?\[([a-z0-9,;]*)\]/Uis';

		foreach ($match[0] as $attributes) {
			preg_match($regExpName, $attributes, $matchName);
			
			// Feld nur speichern und weiter bearbeiten, wenn es das gewählte Prefix hat
			if ($matchName[2]) {
				$this->formFieldNames[] = $matchName[2];
				
				preg_match($regExpTitle, $attributes, $matchTitle);
				$this->formFieldOptions[$matchName[2]] = explode(',', $matchTitle[2]);

				// HTML-Tag bereinigen
				$attributesCleaned = str_replace('['.$matchTitle[2].']', '', $attributes);
				$template = str_replace($attributes, $attributesCleaned, $template);
			}
		}
	
		$this->formTemplate = $template;

		// 2. Feldtypen auslesen
		if (is_array($this->formFieldNames) && $this->formFieldNames[0]) {
			return true;
		} else {
			$this->formFieldNames = array();
			return false;
		}
	}
	
	
	public function setFormData($params){
		foreach($params as $fieldName => $fieldValue){
			$this->formFieldData[$fieldName] = $fieldValue;
		}
	}
	
	/**
	 * public function checkFormData()
	 * Holt alle Formularfelddaten und überprüft diese auf Gültigkeit, sofern dies im Template so eingestellt ist.
	 *
	 * @param void
	 * @return void
	 */
	public function checkFormData() {
	
		foreach ($this->formFieldNames as $formFieldName) {
			
			$data = strip_tags(urldecode(trim($_REQUEST[$formFieldName])));
			$this->formFieldData[$formFieldName] = $data;
			
			if ($this->formFieldIsMandatory($formFieldName)) {

				$type = $this->getFormFieldType($formFieldName);
				
				switch ($type) {
					
					case 'email':
						$check = $this->validateEmail($data);
						break;
						
					case 'zip':
						$check = $this->validateZip($data);
						break;
						
					case 'number':
						$check = $this->validateNumber($data);
						break;

					case 'phone':
						$check = $this->validatePhoneNr($data);
						break;
						
					case 'isset':
						$check = $this->validateExistence($data);
						break;
						
					case 'password':
						$check = $this->validatePassword($data);
						break;
						
					default:
						$check = $this->validateExistence($data);
						break;
				}
				
				$this->formFieldErrors[$formFieldName] = !$check;
				
			}
		}
		
	}
	
	/**
	 * public function formFieldIsMandatory()
	 * Prüft, ob das übergebene Formularfeld erforderlich ist
	 *
	 * @param string $formFieldName Name des Formularfeldes
	 *
	 * @return boolean true => erforderlich, false => optional
	 */
	public function formFieldIsMandatory($formFieldName) {

		if (in_array('mandatory', $this->formFieldOptions[$formFieldName])) {
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * public function getFormFieldType()
	 * Gibt den Datentyp des Formularfeldes zurück, sofern dieser explzit im Template definiert wurde
	 *
	 * @param string $formFieldName Name des Formularfeldes
	 *
	 * @return string Typ des Feldes, z.B. 'email', 'number', 'zip' etc. (mögliche Typen sind definiert in Klassenvariable $this->formFieldTypes)
	 */
	public function getFormFieldType($formFieldName) {
		

		if (!is_array($this->formFieldOptions[$formFieldName])) {
			return '';
		}
		
		$type = array_intersect($this->formFieldTypes, $this->formFieldOptions[$formFieldName]);
		return array_shift($type);
	}
	
	/**
	 * 
	 * public function getFormTemplate()
	 * Gibt das Formulartemplate zurück (welches zuvor in der Methode parseFormFieldsInTemplate geladen worden sein muss.
	 *
	 * @param void
	 * @return string Das Formulartemplate als Zeichenkette
	 */
	public function getFormTemplate() {
		return $this->formTemplate;
	}

	/**
	 * 
	 * public function getFormTemplate()
	 * Gibt die Namen der in dem Template zuvor gefundenen Formularfeldnamen zurück
	 *
	 * @param void
	 * @return array Array mit den gefundenen Formularfeldnamen
	 */
	public function getFormFieldNames() {
		return $this->formFieldNames;
	}

	/**
	 * public function getFormFieldData()
	 * Liefert die eingelesenen Formulardaten zurück
	 *
	 * @return array Formularfelddaten als assoziatives Array (Formularfeldname => Wert)
	 */
	public function getFormFieldData() {
		return $this->formFieldData;
	}

	/**
	 * public function getFormFieldErrors()
	 * Gibt ein Array mit allen Formularfeldern (Schlüssel) und deren Fehlerstatus zurück.
	 *
	 * @param string $suffix Optionale Zeichenkette, die an das Ende des Formularfeldnamens gehängt wird, default ist "Error"
	 * @return array Formularfeldern (Schlüssel) und deren Fehlerstatus(Wert, false = kein fehler oder true = Fehler)
	 */
	public function getFormFieldErrors($suffix='Error') {
		
		$errorArray = array();
		
		foreach ($this->formFieldErrors as $formFieldName => $formFieldValue) {
			$errorArray[$formFieldName.$suffix] = $formFieldValue;
		}

		return $errorArray;
	}
	
	/**
	 * public function formDataHasErrors()
	 * Prüft, ob in den Formulardaten Fehler aufgetaucht sind.
	 *
	 * @param void
	 * @return boolean
	 */
	public function formDataHasErrors() {
		if (in_array(true, $this->formFieldErrors)) {
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * 
	 * public function getExtendedFormFieldData()
	 * Methode kann in abgeleiteten Klassen befüllt werden.
	 *
	 * @params void
	 * @return array Array mit zusätzlichen Parser/Formulardaten als Schlüssel-/Wert-Paare
	 */
	public function getExtendedFormFieldData() {
		return array();
	}
	
	public function checkExtendedFormData() {
		return true;
	}
}
?>