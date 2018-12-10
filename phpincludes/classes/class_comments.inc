<?php

/**
 * class_comments.inc
 * 
 *
 * Langkommentar
 *
 * @version 2013-04-19
 * @author A.Alkaissi <info@buero-hahn.de>
 */
class Comments {

	protected $pageType;	   // dynamic or static
	protected $pageId;		// page id
	protected $articleId;	   // dynamic article id
	protected $templateRow;	   // template of one comment
	protected $templateFrame;	  // template of comments container
	protected $parser;		// parser instance
	protected $db;		 // database instance
	public $errorNr;		// current error number
	public $commentable;	   // show if it allowed to write comment for current page or article
	protected $date;		// current date in YYYY-MM-DD HH:MM:SS format
	protected $defaultApprove;	  // if true, then comments appear immediantly in page (admin verification not need)
	protected $splittString = '{SPLITTDATAHERE}'; // split template string in parts using this splittstring as guide
	protected $params;		// parameters of new submited comment
	protected $defaultAvatarPath;	 // default absulote path to (no picture) avatar image
	protected $maximumCommentLength = 1000;   // maximum length of commentar text, in charachter 
	//public $pageTalbeWrapper;
	protected $waitBeforeSendingNextComment = 2; // standard warte zeit zum nochmal kommentar senden für gleiche Benuzer
	
	/**
	 * Konstruktor
	 */

	public function __construct() {
		$this->date = date('Y-m-d H:i:s');
		$this->parser = new Parser();
		$this->db = new DBCex();
	}

	public function init($params=array()) {
		$defaultParams = array(
			"comments_table" => 'mlog_comments',
			"page_type" => 'dynamic',
			"page_id" => PAGEID,
			"article_id" => '',
			"template_row" => '',
			"template_frame" => '',
			"default_avatar_path" => '',
			"commentsRequestAdminActivation" => false
		);

		$params = array_merge($defaultParams, $params);

		$this->commentsTable = $params['comments_table'];
		$this->pageType = $params['page_type'];
		$this->pageId = $params['page_id'];
		$this->articleId = $params['article_id'];
		$this->templateRow = $params['template_row'];
		$this->templateFrame = $params['template_frame'];
		$this->defaultAvatarPath = $params['default_avatar_path'];

		// default approve, "0" need admin verification to show komment, "1" show all immediantly 
		if ($params['commentsRequestAdminActivation']) {
			$this->defaultApprove = "0";
		} else {
			$this->defaultApprove = "1";
		}

		// find if this page or article accept comments or not
		if ($this->pageType == 'static' && $this->pageId) {
			$this->commentable = 'static';
		} elseif ($this->pageType == 'dynamic' && $this->pageId && $this->articleId) {
			$this->commentable = 'dynamic';
		} else {
			$this->commentable = false;
		}
	}

	/**
	 * public function showCommentsList()
	 *
	 * @param void
	 * @return string Gibt Kommentsliste/ geparste Templates zurueck
	 */
	public function showCommentsList() {
		$query = $this->getCommentsListQuery();
		if ($query) {
			$this->db->query($query);
			$commentsList = '';
			$commentsCount = 0;
			while ($r = $this->db->get(MYSQL_ASSOC)) {
				$this->parser->deleteAllParserVars();
				if ($commentsCount % 2) {
					$this->parser->setParserVar('oddEven', 'Even');
				} else {
					$this->parser->setParserVar('oddEven', 'Odd');
				}

				$this->parser->setParserVar('gravatar', $this->getGravatar($r['comment_author_email'], 32));
				$this->parser->setMultipleParserVars($r);
				$this->parser->setMultipleParserVars($this->formatDate($r['comment_date']));
				$commentsList .= $this->parser->parseTemplate($this->templateRow);
				$commentsCount++;
			}

			
			$this->parser->setParserVar('commentsList', $commentsList);
			$this->parser->setParserVar('commentsCount', $commentsCount);
		}

		// Template Renderen
		$this->parser->setParserVar('articleId', $this->articleId);
		return $this->parser->parseTemplate($this->templatePath . $this->templateFrame);
	}

	/** public function getGravatar()
	 *  get the link to personal avatar of comment writer, depend on his e-mail address
	 * @param $email E-Mail address
	 * @param $default	absolute path to default avatar image (anonym or no picture)
	 * @param $size	size of avatar image in pix
	 * @return string , url of image
	 */
	public function getGravatar($email, $size=40) {
		$grav_url = "http://www.gravatar.com/avatar.php?gravatar_id=" . md5(strtolower($email)) . "&default=" . urlencode($this->defaultAvatarPath) . "&size=" . $size;
		return $grav_url;
	}

	/**
	 * public function formatDate();
	 * 
	 * @param string $datetime
	 * @return array Array mit zwei m�glichen Werten: 'date' => Datum, 'time' => Uhrzeit
	 */
	public function formatDate($datetime) {
		$monateNamen = array("Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember");
		preg_match('/^(\d{4})-(\d{2})-(\d{2})(\s(\d{2}):(\d{2}):(\d{2}))?$/', $datetime, $match);
		$r['date'] = $match[3] . '. ' . $monateNamen[intval($match[2]) - 1] . '  ' . $match[1];
		$r['time'] = $match[5] . '.' . $match[6];
		return $r;
	}

	/** public function getCommentsListQuery()
	 * 
	 * @return string, query
	 */
	public function getCommentsListQuery() {
		// Articleliste in list view Mode
		if ($this->commentable == 'static') {
			$query = "SELECT * FROM " . $this->commentsTable . " WHERE comment_pid='" . $this->db->mysqlQuote($this->pageId) . "' AND comment_approved='1' ORDER BY comment_date  ASC";
		} elseif ($this->commentable == 'dynamic') {
			$query = "SELECT * FROM " . $this->commentsTable . " WHERE comment_pid='" . $this->db->mysqlQuote($this->pageId) . "' AND comment_approved='1' AND comment_aid='" . $this->db->mysqlQuote($this->articleId) . "' ORDER BY comment_date  ASC";
		} else {
			return false;
		}
		return $query;
	}

	/**
	 * protected function getArticlescommentsTotal
	 * get the comments summe of an article by its id
	 * @param $aid, article id
	 * @return int, comments summe
	 * */
	public function getCommentsCountOfArticle($aid) {
		if (!PAGEID || !$aid || !$this->commentsTable) {
			return 0;
		}
		$query = "SELECT count(*) as commentsTotal FROM " . $this->commentsTable . " WHERE comment_pid='" . $this->db->mysqlQuote($this->pageId) . "' AND comment_approved='1' AND comment_aid='" . $this->db->mysqlQuote($aid) . "' ORDER BY comment_date  DESC";

		$this->db->query($query);
		if ($r = $this->db->get(MYSQL_ASSOC)) {
			return $r['commentsTotal'];
		}
		return 0;
	}

	/**
	 * protected function getLastComments
	 * get the last comments  of an article by its id
	 * @param $aid, article id
	 * @return array, last comments
	 * */
	public function getLastCommentsOfArticle($aid, $lastCommentsLimit=3) {
		if (!$this->pageId || !$aid || !$this->commentsTable) {
			return array();
		}
		$lastComments = array();
		$query = "SELECT * FROM " . $this->commentsTable . " WHERE comment_pid='" . $this->db->mysqlQuote($this->pageId) . "' AND comment_approved='1' AND comment_aid='" . $this->db->mysqlQuote($aid) . "' ORDER BY comment_date  DESC LIMIT 0," . $lastCommentsLimit;
		$this->db->query($query);
		while ($r = $this->db->get(MYSQL_ASSOC)) {
			$r['gravatar'] = $this->getGravatar($r['comment_author_email'], 32);
			foreach ($this->formatDate($r['comment_date']) as $index => $value) {
				$r[$index] = $value;
			}
			$lastComments[] = $r;
		}
		return $lastComments;
	}

	/**
	 * public function setContentTable()
	 * Setter: Setzt klassenintern die Variable 'contentTable'
	 *
	 * @param $name string Datei-/Verzeichnis-/Pfadname
	 * @return string Es wird ein korrekter Name zurueckgegeben
	 */
	public function getSearchParams() {
		$params = '';
		foreach ($_GET as $param => $value) {
			$noSearchParams = array("cp", "pid", "lang", "aid", "action");
			if (!$value || in_array($param, $noSearchParams))
				continue;
			$params .= "&" . $param . "=" . $value;
		}
		return $params;
	}

	/**
	 * public function getSearchParamsForPrintLink()
	 * Erzeugt den Querystring f�r den Druckansichtlink jeder Seite und definiert ihn als Konstante PRINTLINKPARAMS
	 *
	 * @param void
	 * @return string Parameterstring oder Leer-String
	 */
	public function getSearchParamsForPrintLink() {

		if (defined('PRINTLINKPARAMS'))
			return;

		$params = '';

		foreach ($_GET as $param => $value) {
			$noSearchParams = array('lang', 'pid');
			if (!$value || in_array($param, $noSearchParams))
				continue;
			$params .= '&amp;' . $param . "=" . $value;
		}
		define('PRINTLINKPARAMS', $params);
		return $params;
	}

	/** public function isSpam()
	 * comments for the same person(depend on ip number) will allow after ## secounds of time.
	 * @param $timer, integer 
	 * @return bool
	 */
	public function isSpam($timer=180) {
		$dateBefore3Min = date('Y-m-d H:i:s', strtotime('- ' . $timer . ' seconds', strtotime($this->date)));
		$query = "SELECT count(id) as count FROM " . $this->commentsTable . " Where comment_author_ip='" . $this->db->mysqlQuote($this->getRefererIp()) . "' AND comment_date > '" . $dateBefore3Min . "' ";
		
		$this->db->query($query);
		$r = $this->db->get();
		if ($r['count']) {
			$this->errorNr = 3017;
			return true;
		}
		return false;
	}

	/** function isValid()
	 * validate sending comment form
	 * @param $params array of parameters
	 * 		//author
	 * 		//email
	 * 		//url
	 * 		//comment
	 * 		//notify
	 * @return bool
	 */
	public function isValid($params=array()) {
		
		if ($this->isSpam($this->waitBeforeSendingNextComment))
			return false;
		if (!$this->checkAuthor($params['author']))
			return false;
		if (!$this->checkEmail($params['email']))
			return false;
		if (!$this->checkComment($params['comment']))
			return false;
		if (!$this->checkCaptcha($params['captchaVarName']))
			return false;
		if ($this->pageType == 'dynamic') {
			if (!$this->checkCommentPostId($params['comment_post_ID']))
				return false;
		}
		$notifySender = ($params['notify']) ? 1 : 0;
		$this->createHash();
		$params['currentHash'] = $this->currentHash;
		$params['commentDate'] = $this->date;
		$query = "INSERT INTO " . $this->commentsTable . " SET comment_hash = '" . $this->currentHash . "', ";
		$query .="comment_pid ='" . $this->pageId . "', ";
		if ($params['comment_post_ID']) {
			$query .="comment_aid ='" . $params['comment_post_ID'] . "', ";
		}
		$query .="comment_author ='" . $this->db->mysqlQuote($params['author']) . "', ";
		$query .="comment_title ='" . $this->db->mysqlQuote($params['commentTitle']) . "', ";
		$query .="comment_author_email ='" . $this->db->mysqlQuote($params['email']) . "', ";
		$query .="comment_author_url ='" . $this->db->mysqlQuote($params['author_url']) . "', ";
		$query .="comment_author_ip ='" . $this->getRefererIp() . "', ";
		$query .="comment_approved ='" . $this->defaultApprove . "', ";
		$query .="comment_content ='" . $this->db->mysqlQuote($params['comment']) . "', ";
		$query .="comment_date ='" . $this->date . "', ";
		$query .="comment_notify_sender ='" . $notifySender . "' ";

		$this->db->query($query);

		if ($this->db->lastErrorNr) {
			$this->errorNr = 3004;
			return false;
		}

		$this->params = $params;
		return true;
	}

	
	 /* function captcha()
 	 * Prüft eine captcha auf ihre Gültigkeit
 	 * 
 	 * @param string captcha
 	 * @return boolean true oder false, je nach Ergebnis der Prüfung
 	 */
	 	public function checkCaptcha($captcha) {
	 		$captcha = trim($captcha);
	 		if ($captcha && $this->getCaptchaValue('mlogCaptcha') == $captcha) {
	 			return true;
	 		} else {
	 			$this->errorNr = 3014;
	 			return false;
	 		} 
	 	}	 	

  	/**
 	 * function getCaptchaValue() 
 	 * Gibt den Wert der aktuellen Captcha-Grafik als String zurück
 	 * 
 	 * @param void
 	 * 
 	 * @return string 6-stelliger Zeichencode
 	 */
	public function getCaptchaValue($captchaVarName) {
		
		if (!is_object($this->session)) $this->session = new Session();
		return $this->session->getSessionVar($captchaVarName);
	}
	
	public function resetCaptcha(){
		$this->session->DeleteSessionVar('mlogCaptcha');
		$this->session->SaveSessionVars();
	}
	/**
	 * function  getRefererIp()
	 * Ermittelt die IP-Adresse des Referers
	 * 
	 * @param void
	 * @return string IP des aufrufenden Computers
	 */
	private function getRefererIp() {
		$createdByIp = getenv('HTTP_X_FORWARDED_FOR');
		if (!$createdByIp)
			$createdByIp = getenv('REMOTE_ADDR');
		return $createdByIp;
	}

	/**
	 * function activateComment() 
	 * Setzt den Status eines kommentar auf 'aktiv'
	 * 
	 * @param array $params Parameter werden per Array übergeben
	 * @param string $hash Hash-Wert, anhand dessen der User ermittelt werden kann
	 * 
	 * @return bool Liefert true oder false, je nachdem, ob die Aktivierung erfolgreich war
	 */
	function activateComment($params) {
		$commentId = $this->getCommentIdFromHash($params['hash']);
		
		
		if (!$commentId) {
			$this->errorNr = 30;
			return false;
		} else {
			
			$this->db->query("SELECT * FROM mlog_comments WHERE  comment_approved <>1 AND id = '" . $this->db->mysqlQuote($commentId) . "'");
			
			$res = $this->db->get();

			if (!$res || empty($res)) {
				$this->errorNr = 20;				
				return false;
			}
			
			$this->db->query("UPDATE mlog_comments SET  comment_approved = '1' WHERE id = '" . $this->db->mysqlQuote($commentId) . "'");
			if ($this->db->lastErrorNr) {
				$this->errorNr = 30;
				return false;
			} else {
				$this->errorNr = 10;
				return true;
			}
		}
	}

	/**
	 * function deleteComment() 
	 * löscht einer kommentar
	 * 
	 * @param array $params Parameter werden per Array übergeben
	 * @param string $hash Hash-Wert, anhand dessen der User ermittelt werden kann
	 * 
	 * @return bool Liefert true oder false, je nachdem, ob die enfernung erfolgreich war
	 */
	function deleteComment($params) {
		$commentId = $this->getCommentIdFromHash($params['hash']);
		if (!$commentId) {
			$this->errorNr = 50;
			return false;
		} else {
			$this->db->query("DELETE FROM mlog_comments WHERE id = '" . $this->db->mysqlQuote($commentId) . "'");
			if ($this->db->lastErrorNr) {
				$this->errorNr = 60;
				return false;
			} else {
				$this->errorNr = 40;
				return true;
			}
		}
	}

	/**
	 * function getCommentIdFromHash($hash) 
	 * get comment id by hash field value
	 * 
	 * @param string $hash hash value
	 * 
	 * @return int comment id
	 */
	function getCommentIdFromHash($hash) {
		$commentId = 0;
		if ($hash) {
			$this->db->query("SELECT id FROM mlog_comments  WHERE comment_hash = '" . $this->db->mysqlQuote($hash) . "' LIMIT 1");
		
			while ($res = $this->db->get()) {
				$commentId = $res['id'];
			}
		}
		return $commentId;
	}

	/**
	 * function checkAuthor()
	 * Prüft eine Author auf ihre Gültigkeit
	 * 
	 * @param string Author
	 * @return boolean true oder false, je nach Ergebnis der Prüfung
	 */
	public function checkAuthor($author) {
		if ($author) {
			return true;
		} else {
			$this->errorNr = 3012;
			return false;
		}
	}

	/**
	 * function checkComment()
	 * Prüft eine Comment auf ihre Gültigkeit
	 * 
	 * @param string Comment
	 * @return boolean true oder false, je nach Ergebnis der Prüfung
	 */
	public function checkComment($comment) {
		if ($comment) {
			if (strlen($comment) > $this->maximumCommentLength) {
				$this->errorNr = 3019;
				return false;
			}
			return true;
		} else {
			$this->errorNr = 3013;
			return false;
		}
	}

	protected function checkCommentPostId($commentId) {
		$commentId = trim(intval($commentId));
		if ($commentId) {
			return true;
		} else {
			$this->errorNr = 3033;
			return false;
		}
	}

	/**
	 * function checkEmail()
	 * Prüft eine E-Mailadresse auf ihre Gültigkeit
	 * 
	 * @param string email E-Mailadresse
	 * @return boolean true oder false, je nach Ergebnis der Prüfung
	 */
	public function checkEmail($email) {
		$email = trim($email);
		if (!$email) {
			$this->errorNr = 3016;
			return false;
		}
		$isValid = true;
		$atIndex = strrpos($email, "@");
		if ((is_bool($atIndex) && !$atIndex)) {
			$isValid = false;
			$this->errorNr = 3001;
		} else {
			$domain = substr($email, $atIndex + 1);
			$local = substr($email, 0, $atIndex);
			$localLen = strlen($local);
			$domainLen = strlen($domain);
			if ($localLen < 1 || $localLen > 64) {
				// local part length exceeded
				$isValid = false;
			} else if ($domainLen < 1 || $domainLen > 255) {
				// domain part length exceeded
				$isValid = false;
			} else if ($local[0] == '.' || $local[$localLen - 1] == '.') {
				// local part starts or ends with '.'
				$isValid = false;
			} else if (preg_match('/\\.\\./', $local)) {
				// local part has two consecutive dots
				$isValid = false;
			} else if (!preg_match('/^[A-Za-z0-9\\-\\.]+\.[A-Za-z]+$/', $domain)) {
				// character not valid in domain part
				$isValid = false;
			} else if (preg_match('/\\.\\./', $domain)) {
				// domain part has two consecutive dots
				$isValid = false;
			} else if (!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\", "", $local))) {
				// character not valid in local part unless 
				// local part is quoted
				if (!preg_match('/^"(\\\\"|[^"])+"$/', str_replace("\\\\", "", $local))) {
					$isValid = false;
				}
			}
			if ($isValid == false) {
				$this->errorNr = 3001;
			}
		}
		return $isValid;
	}

	/**
	 * function createHash ()
	 * Erzeugt einen eindeutigen Hash-Wert/-String
	 * 
	 * @param void
	 * @return string Hash-Wert
	 */
	public function createHash() {
		$this->currentHash = md5(uniqid(rand(), true));
		return $this->currentHash;
	}

	/**
	 * function deleteHash() 
	 * Löscht Hash-Wert/-String eines Users
	 * 
	 * @param array $params Parameter werden per Array übergeben
	 * @param number $subscriberId ID des Benutzers
	 * 
	 * @return bool false, wenn keine User-ID übergeben wurde, ansonsten true
	 */
	public function deleteHash($params) {
		if (!$params['subscriberId'])
			return false;

		$this->db->query("UPDATE paperboy_subscribers SET action_hash = '' WHERE id = '" . $this->db->mysqlQuote($params['subscriberId']) . "'");
		return true;
	}

	/**
	 * function checkFile() 
	 * Prüft, ob eine Datei vorhanden ist
	 * 
	 * @param string $file Dateiname (inkl. Pfad)
	 * @return boolean true oder false, je nach Ergebnis der Prüfung
	 */
	public function checkFile($file) {
		if (file_exists($file)) {
			return true;
		} else {
			$this->errorNr = 3006;
			return false;
		}
	}

	/**
	 * function notifyAdministrator()
	 * Versendet eine Benachrichtigungsmail an Administrator
	 * 
	 * @param array $params Folgende Parameter werden per Array übergeben:
	 * @param string email E-Mailadresse des Empfänger
	 * @param string template Pfad zum Template
	 * 
	 * @return bool true oder false, je nachdem ob die E-Mail versendet werden konnte 
	 */
	public function notifyAdministrator($params) {

		if (!$this->checkFile($params['template']))
			return false;

		$mailTemplate = file_get_contents($params['template']);
		$mailTemplateParts = (explode($this->splittString, $mailTemplate));

		// Sollte ergeben:
		// 0. Absendername
		// 1. Absendermail
		// 2. Anwortadresse
		// 3. Betreffzeile
		// 4. Textkörper
		// 5. HTML-Körper
		// Alle Parameter werden an den Parser übergeben/ durchgeschleift
		$senderMail = trim($mailTemplateParts[1]);
		$params['adminEmail'] = $senderMail;
		
		$this->parser->setMultipleParserVars($params);
		$this->parser->setMultipleParserVars($this->params);

		// Teile erzeugen/ parsen
		$partSubject = $this->parser->parse($mailTemplateParts[3]);
		$partText = $this->parser->parse(trim($mailTemplateParts[4]));
		$partHtml = $this->parser->parse(trim($mailTemplateParts[4]));

		// E-Mail erzeugen und versenden
		require_once(PATHTOADMIN . 'classes/class_mimemailer.php');
		$mimeMailer = new MimeMailer();
		$mimeMailer->eol = "\n";
		$mimeMailer->createMail(array('html' => utf8_decode($partHtml),
			'text' => utf8_decode($partText),
			'attachments' => $params['attachments'],
			'senderMail' => $senderMail,
			'senderName' => $mailTemplateParts[0],
			'replyTo' => $mailTemplateParts[2]
		));
/*
		$f = fopen('adminMail.txt','a');
			fwrite($f, $mimeMailer->mailBody);
		fclose($f);

 * 
 */		
		$check = mail($senderMail, $partSubject, $mimeMailer->mailBody, $mimeMailer->mailHeader, '-f' . $mailTemplateParts[1]);
	}

	/**
	 * function notifyAdministrator()
	 * Versendet eine Benachrichtigungsmail an Administrator
	 * 
	 * @param array $params Folgende Parameter werden per Array übergeben:
	 * @param string email E-Mailadresse des Empfänger
	 * @param string template Pfad zum Template
	 * 
	 * @return bool true oder false, je nachdem ob die E-Mail versendet werden konnte 
	 */
	public function notifyFollowers($params) {
		if (!$this->checkFile($params['template']))
			return false;

		$mailTemplate = file_get_contents($params['template']);
		$mailTemplateParts = (explode($this->splittString, $mailTemplate));

		// Sollte ergeben:
		// 0. Absendername
		// 1. Absendermail
		// 2. Anwortadresse
		// 3. Betreffzeile
		// 4. Textkörper
		// 5. HTML-Körper
		// Alle Parameter werden an den Parser übergeben/ durchgeschleift
		$this->parser->setMultipleParserVars($params);
		$this->parser->setMultipleParserVars($this->params);
		// Teile erzeugen/ parsen
		$partSubject = $this->parser->parse($mailTemplateParts[3]);
		$partText = $this->parser->parse(trim($mailTemplateParts[4]));
		$partHtml = $this->parser->parse(trim($mailTemplateParts[4]));

		// E-Mail erzeugen und versenden
		require_once(PATHTOADMIN . 'classes/class_mimemailer.php');
		$mimeMailer = new MimeMailer();
		$mimeMailer->eol = "\n";
		$mimeMailer->createMail(array('html' => utf8_decode($partHtml),
			'text' => utf8_decode($partText),
			'attachments' => $params['attachments'],
			'senderMail' => $mailTemplateParts[1],
			'senderName' => $mailTemplateParts[0],
			'replyTo' => $mailTemplateParts[2]
		));
		$query = "SELECT * FROM " . $this->commentsTable . " WHERE comment_aid='" . $this->db->mysqlQuote($this->articleId) . "' ";
		$query .= "AND comment_author_email <>'" . $params['userEmail'] . "' ";
		$query .= "AND comment_pid='" . $this->pageId . "' AND comment_notify_sender='1' AND comment_approved='1' GROUP BY comment_author_email";
		
		$this->db->query($query);
		while ($r = $this->db->get()) {
			$check = @mail($r['comment_author_email'], $partSubject, $mimeMailer->mailBody, $mimeMailer->mailHeader, '-f' . $mailTemplateParts[1]);
		/*
			$f = fopen('userMail.txt','a');
				fwrite($f, $r['comment_author_email']);
				fwrite($f, $mimeMailer->mailBody);
			fclose($f);
		 * 
		 */
		}
	}

}

?>
