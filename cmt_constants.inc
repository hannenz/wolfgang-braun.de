<?php
/**
 * Conten-o-mat Einstellungen/Konstanten
 */

/**
 * Change this
 */
// Name des Webs
define ('WEBNAME', 'Wolfgang Braun');

// PHP Namespace
define('APP_NAMESPACE', 'WolfgangBraun');

// Error reporting level: One of 'all', 'warning', 'strict' or 'error'
define ('CMT_ERRORREPORTING_LEVEL', 'error');

/**
 * Localasation settings
 */
// Standardsprache für mehrsprachige Websites
define ('DEFAULTLANGUAGE', 'de');

// veraltet: Zeichensatz, der für die Seite verwendet wird
define ('CHARSET', 'utf-8');

// Zeichensatz, der verwendet wird, falls kein anderer Zeichensatz angegeben wird
define ('CMT_DEFAULTCHARSET', 'utf-8');

// Standardzeitzone für PHP-Funktion 'date'
define('CMT_DEFAULTTIMEZONE', 'Europe/Berlin');


/**
 * Additional Settings
 */
define('CMT_DEFAULT_DATABASE', 'default');

// Set environment in .htacces, e.g. 
// ```
// <IfModule mod_env.c>
//   SetEnv CMT_RUNTIME_ENVIRONMENT production
// </IfModule>
// or as a shell environment variable
// $ CMT_RUNTIME_ENVIRONMENT=dev php index.php controller action
if (isset($_SERVER['CMT_RUNTIME_ENVIRONMENT'])) {
	$cmtRuntimeEnv = $_SERVER['CMT_RUNTIME_ENVIRONMENT'];
}
else if (getenv('CMT_RUNTIME_ENVIRONMENT')) {
	$cmtRuntimeEnv = getenv('CMT_RUNTIME_ENVIRONMENT');
}
else {
	$cmtRuntimeEnv = '';
}
define('CMT_RUNTIME_ENVIRONMENT', $cmtRuntimeEnv);

define ('CLI', php_sapi_name() == 'cli');

// Datenbankfehler Logging
define ('CMT_DBERRORLOG', '1');

// Sollen Cookies benutzt werden?
define('CMT_USECOOKIES', '1');

// Cookies nutzen, auch wenn keine Cookies aktiviert sind?
define('CMT_FORCECOOKIES', '1');

// Apache Mod-Rewrite nutzen?
define('CMT_MODREWRITE', '1');

// Base Path for REST API. Comment to disable REST API. Must begin with a slash
// and match the settings in .htaccess!
define ('CMT_REST_API_BASE_PATH', '/cmtrestapi');



/**
 * Paths and Directories
 */
// Pfad, in dem sich der ROOT der Homepage befindet (z.B. "/" bei www.meinedomain.de (Standard) oder 
// "myweb/" bei www.meinedomain/myweb/), immer mit führendem und ggf. abschließendem '/' angeben!
define ('WEBROOT', '/');

// Pfad/Ordner zum Admin-Bereich: Standard 'admin/'
define ('ADMINPATH', 'contentomat/');

// Zentraler Ordner für Downloads: Standard ist 'downloads/'
define ('DOWNLOADPATH', 'downloads/');

// URL des Skriptes
define ("SELF", basename($_SERVER['PHP_SELF']));

// aktueller Pfad minus WEBROOT ergibt den aktuellen ROOT
// $actPath = dirname(__FILE__) . '/';  //preg_replace('/^'.preg_quote(WEBROOT, '/').'/', '', $_SERVER['PHP_SELF']);

// ROOT berechnen
// $depth = substr_count ($actPath, '/');
// define ('ROOT', str_pad('', $depth*3, '../'));
define ('ROOT', dirname(__FILE__) . '/');
// PATHTOADMIN berechnen
define ('PATHTOADMIN', str_replace('//', '/', ROOT.ADMINPATH));

// PATHTODOWNLOADS berechnen
define ('PATHTODOWNLOADS', ROOT.DOWNLOADPATH);

// PATHTOWEBROOT berechnen
define ('PATHTOWEBROOT', ROOT);

// Include-Pfade für PHP-Skripte (im Idealfall immer absolut angeben!):
// Include-Pfad zum Webroot

// $includePath = empty($_SERVER['DOCUMENT_ROOT']) ? '' : ($_SERVER['DOCUMENT_ROOT'] . DIRECTORY_SEPARATOR);
// define ('INCLUDEPATH', $includePath);
define('INCLUDEPATH', __DIR__ . DIRECTORY_SEPARATOR);

// Include-Pfad zum Adminbereich
define ('INCLUDEPATHTOADMIN', rtrim(INCLUDEPATH, DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR . trim(ADMINPATH, DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR);

define ('PATHTOTMP', PATHTOWEBROOT . 'tmp' . DIRECTORY_SEPARATOR);
require_once(PATHTOADMIN . 'includes' . DIRECTORY_SEPARATOR . 'logger.inc');
// Log Level (see admin/classes/class_logger.php for available values and their meanings)
// Only log messages with this or higher log level
define ('CMT_LOG_LEVEL', LOG_LEVEL_ALL);

// Log Target(s)
define ('CMT_LOG_TARGET', LOG_TARGET_FILE);

// Path to Log File, if log target LOG_TARGET_FILE is selected (must be writable by web server user)
define ('CMT_LOG_FILE', PATHTOTMP . 'logs' . DIRECTORY_SEPARATOR . 'contentomat.log');
?>
