<?php 
/**
 * class_data_validation.inc
 * Prüft Daten auf Ihre Gueltigkeit
 * 
 * @version 2011-03-11
 * @author J.Hahn <info@buero-hahn.de>
 */

	class DataValidation {
		
		protected $defaultCountry;
		
		public function __construct() {
			$this->defaultCountry = 'de';
		}
		
				
		/**
		 * public function setCountry()
		 * Setter: Setzt Klassenvariable $defaultCountry
		 * 
		 * @param string $country Laenderkuerzel
		 * @return void
		 */	
		public function setCountry($country) {
			$this->defaultCountry = $country;
		}
		
		
		/**
		 * public function validateZip()
		 * Validiert Postleitzahlen
		 * 
		 * @param string $zip Postleitzahl, z.B. '70176' oder 'A-2346'
		 * @param string $country Landeskuerzel, z.B. 'D', 'A' ... 
		 * 
		 * @return boolean True oder false, abhaengig ob PLZ gueltig oder nicht
		 */		
		public function validateZip($zip, $country='') {
			
			if (!$country) {
				$country = $this->defaultCountry;
			}
		
			switch (strtolower($country)) {
				
				// Deutschland
				case 'de':
				case 'd':
					
					// Event. fuehrendes 'D-' entfernen
					$zip = trim(preg_replace('/^\s*D\s*-\s*/is', '', $zip));
					
					// Leerzeichen entfernen
					$zip = preg_replace('/[\s\t\r\n]/s', '', $zip);

					// 5 Zeichen Laenge ist Pflicht
					if (!preg_match('/^\d{5}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;

				// Österreich
				case 'at':
				case 'a':
					
					// Event. fuehrendes 'A-' entfernen
					$zip = trim(preg_replace('/^\s*A\s*-\s*/is', '', $zip));
					
					// Leerzeichen entfernen
					$zip = preg_replace('/[\s\t\r\n]/s', '', $zip);
					
					// 4 Zeichen Laenge ist Pflicht
					if (!preg_match('/^\d{4}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;

				// Schweiz
				case 'ch':
					// Event. fuehrendes 'CH-' entfernen
					$zip = trim(preg_replace('/^\s*CH\s*-?\s*/is', '', $zip));
					
					// Leerzeichen entfernen
					$zip = preg_replace('/[\s\t\r\n]/s', '', $zip);
					
					// 4 Zeichen Laenge ist Pflicht
					if (!preg_match('/^\d{4}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					
					break;
					
				// GB England
				case 'gb':
				case 'en':
					
					// Event. WhiteSpace am Anfang und Ende entfernen
					$zip = trim($zip);
					
					// Leerzeichen entfernen
					// $zip = preg_replace('/[\s\t\r\n]/s', '', $zip);
					
					// Quelle: http://regexlib.com/REDetails.aspx?regexp_id=1047  Comment 10/23/2008 5:52:56 AM
					// if (!preg_match('/^((([BEGLMNS][1-9]\d?)|(W[2-9])|((A[BL]|B[ABDHLNRST]|C[ABFHMORTVW]|D[ADEGHLNTY]|E[HNX]|F[KY]|G[LUY]|H[ADGPRSUX]|I[GMPV]|JE|K[ATWY]|L[ADELNSU]|M[EKL]|N[EGNPRW]|O[LX]|P[AEHLOR]|R[GHM]|S[AEGKL-PRSTWY]|T[ADFNQRSW]|UB|W[ADFNRSV]|YO|ZE)\d\d?)|(W1[A-HJKSTUW0-9])|(((WC[1-2])|(EC[1-4])|(SW1)|E1|W1)[ABEHMNPRVWXY]))(\s*)?([0-9][ABD-HJLNP-UW-Z]{2}))$|^(GIR\s?0AA)$|^(ASCN|BBND|BIQQ|FIQQ|PCRN|SIQQ|STHL|TDCU|TKCA)\s?1ZZ$|^(BFPO\s?\d\d?\d?)$/', $zip)) {
					
					if (!preg_match('/^[A-Z]{1,2}[0-9R][0-9A-Z]? [0-9][ABD-HJLNP-UW-Z]{2}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;
					
				
				// Frankreich
				case 'fr':
				case 'f':
					
					// Event. fuehrendes 'F-' entfernen
					$zip = trim(preg_replace('/^\s*F\s*-\s*/is', '', $zip));
					
					// Leerzeichen entfernen
					$zip = preg_replace('/[\s\t\r\n]/s', '', $zip);
					
					// http://regexlib.com/REDetails.aspx?regexp_id=203
					if (!preg_match('/^((2[A|B])|[0-9]{2})[0-9]{3}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;
					
					
				// Spanien				
				case 'es':        
					
					// Leerzeichen entfernen
					$zip = trim(preg_replace('/[\s\t\r\n]/s', '', $zip));
					
					// 19444 | 09444 | 10444  aber nicht 00444  
					if (!preg_match('/^([1-9]{2}|[0-9][1-9]|[1-9][0-9])[0-9]{3}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;
					
				// Italia	
				case 'it':     
				
					// Leerzeichen entfernen
					$zip = trim(preg_replace('/[\s\t\r\n]/s', '', $zip));
					
					// http://www.dbsoftlab.com/Regular-Expressions-/Is-Italian-Post-Code.html
					if (!preg_match('/^(V-|I-)?\d{4}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;
					
				// Niederlande
				case 'nl':    
				
					// Leerzeichen entfernen
					$zip = trim(preg_replace('/[\s\t\r\n]/s', '', $zip));
					
					if (!preg_match('/^[1-9][0-9]{3}\s?[a-zA-Z]{2}$/', $zip)) {
						return false;
					} else {
						return true;
					}
					break;
					
					
				default:
					// trigger_error("Fehler! Nicht definierter Laenderkurzel - validateZIP", E_USER_WARNING);
					return false;
					break;	
					
			}
			
			return false;
		}
		
		/**
		 * public function validatePhoneNr()
		 * Validiert Telefonnummern
		 * 
		 * @param string $phone Telefonnummer, z.B. '+49 711 123456' oder '040/20304050'
		 * @param string $country Landeskuerzel, z.B. 'D', 'A' ... 
		 * 
		 * @return boolean True oder false, abhaengig ob Nummer gueltig oder nicht
		 */		
		public function validatePhoneNr($phone, $country='') {

			if (!$country) {
				$country = $this->defaultCountry;
			}
			
			switch (strtolower($country)) {
				
				// Alle Laender
				case 'de':
				case 'at':
				default:

					// Leerzeichen, Tabs, Zeilen-Umbrueche entfernen
					$phone = trim(preg_replace('/[\s\t\r\n]/s', '', $phone));
					
					// 11.11.2009 rm - doch nicht hier - eher in cleanupPhoneNr() - oder
					// Doppelte Trennzeichen ..//-- entfernen
					// $phone = preg_replace('/[[:punct:]]([[:punct:]])+/s', '$1', $phone);
					// Am Anfang und Ende alle Zeichen wie .,-/ entfernen
					// $phone = preg_replace(array('/([[:punct:]]*)$/s', '/^([^0-9\+]*)/s'), '', $phone);
					
					// if (!preg_match('/^[+\d-\/.]+$/s', $phone)) {
					// +49(711)323-1231 
					// -> http://www.pagecolumn.com/tool/pregtest.htm
					
					// mehrere ./- in Folge kann/soll nicht sein -> false
					if( preg_match('/^.*[-\/\.]{2,}.*$/s', $phone))
						return false;
						
					// if (!preg_match('/^((\+)?(\d+))*(\(\d+\))*\d{2}[\d-\/.]+\d{2}$/', $phone)) {
					// if (!preg_match('/^((\+)?(\d+))*(\(\d+\))*(\d{2,}[\d-\/\.]+\d{2})|(\d{7,})$/', $phone)) {
					// if( !preg_match('/^((\+[1-9]\d*)*\d+)*(\(\d+\))*(\d{2,})|(\d{7,})$/', $phone)) {
					// if( !preg_match('/^((\+[1-9]\d*)*\d+)*(\(\d+\))*\d{2,})$/s', $phone)) {
					
					if( !preg_match('/^(\+[1-9])*((\(\d+\))(([\d-\/\.]{2,}\d{2,}))|([\d-\/\.]{6,}\d{2,})|(\d{6,}))$/s', $phone)) {
						return false;
					} else {
						return true;
					}
					break;
			}
			
			return false;
		}
		

		/**
		 * public function validateNumber()
		 * Validiert Zahlen
		 * 
		 * @param string $number Zahl
		 * 
		 * @return boolean True oder false, abhängig ob Nummer gültig oder nicht
		 */		
		public function validateNumber($number, $length=0) {
			
			$number = trim($number);
			if ($length) {
				$addLength = '{'.intval($length).'}';
			} else {
				$addLength = '+';
			}
			if (preg_match('/^[\d]'.$addLength.'$/', $number)) {
				return true;
			} else {
				return false;
			}

		}
		
		/**
	 	 * function validateEmail() 
		 * Prüft E-Mailadresse auf Gueltigkeit
		 * von: http://www.linuxjournal.com/article/9585
	 	 * 
	 	 * @param string $email E-Mailadresse
	 	 * @return bool True oder false (gueltig oder ungueltig)
	 	 */
		
		public function validateEmail($email) {
			$isValid = true;
			$atIndex = strrpos($email, "@");
			if (is_bool($atIndex) && !$atIndex) {
				$isValid = false;
			} else {
				$domain = substr($email, $atIndex+1);
				$local = substr($email, 0, $atIndex);
				$localLen = strlen($local);
				$domainLen = strlen($domain);
				if ($localLen < 1 || $localLen > 64) {
					// local part length exceeded
					$isValid = false;
				} else if ($domainLen < 1 || $domainLen > 255) {
					// domain part length exceeded
					$isValid = false;
				} else if ($local[0] == '.' || $local[$localLen-1] == '.') {
					// local part starts or ends with '.'
					$isValid = false;
				} else if (preg_match('/\\.\\./', $local)) {
					// local part has two consecutive dots
					$isValid = false;
				} else if (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain)) {
					// character not valid in domain part
					$isValid = false;
				} else if (preg_match('/\\.\\./', $domain)) {
					// domain part has two consecutive dots
					$isValid = false;
				} else if (!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\","",$local))) {
					// character not valid in local part unless 
					// local part is quoted
					if (!preg_match('/^"(\\\\"|[^"])+"$/', str_replace("\\\\","",$local))) {
						$isValid = false;
					}
				}
				
				// DNS-Check
				if(function_exists('checkdnsrr')) {
					if ($isValid && !(checkdnsrr($domain,"MX") || checkdnsrr($domain,"A"))) {
						// domain not found in DNS
						$isValid = false;
					}
				}
			}
			
			return $isValid;
		}

		/**
		 * public function validateExistence()
		 * Prüft einfach nur, ob die Variable leer ist oder nicht
		 *
		 * @param string $data
		 *
		 * @return boolean
		 */
		public function validateExistence($data) {
			$data = trim($data);
			
			if (!$data) {
				return false;
			} else {
				return true;
			}
		}
		
		/**
		 * public function validatePassword()
		 * Prüft ein Passwort auf Gültigkeit. In diesem Fall wird nur die Länge auf mind. 8 zeichen geprüft.
		 *
		 * @param string $password Das Passwort
		 *
		 * @return boolean
		 */
		public function validatePassword($password) {
			
			$password = trim($password);
			if (strlen($password) < 8) {
				return false;
			} else {
				return true;
			}
		}
	}  //  End-of-class  DataValidation
	
?>