/**
 * Default gulpfile for HALMA projects
 *
 * Version 2021-05-20
 *
 * @see https://www.sitepoint.com/introduction-gulp-js/
 * @see https://nystudio107.com/blog/a-gulp-workflow-for-frontend-development-automation
 * @see https://nystudio107.com/blog/a-better-package-json-for-the-frontend
 *
 * @usage
 * gulp : Start browser-sync, watch css and js files for changes and run development builds on change
 * gulp build : run a development build
 * gulp build --production : run a production build
 * gulp --tasks : Show available tasks (if you want to run a single specific task)
 */
'use strict';

// Read command line paramters (arguments)
const argv = require('yargs').argv;

// Check if we want a prodcution build
// Call like `gulp build --production` (or a single task instead of `build`)
const isProduction = (argv.production !== undefined);

// package vars
const pkg = require('./package.json');
const fs = require('fs');

// gulp
const gulp = require('gulp');
const {series} = require('gulp');

// Load all plugins in 'devDependencies' into the variable $
const $ = require('gulp-load-plugins')({
	pattern: ['*'],
	scope: ['devDependencies'],
	rename: {
		'gulp-strip-debug': 'stripdebug',
		'fancy-log' : 'log'
	}
});


// Default error handler: Log to console
const onError = (err) => {
	console.log(err);
};

// A banner to output as header for dist files
const banner = [
	"/**",
	" * @project       <%= pkg.name %>",
	" * @author        <%= pkg.author %>",
	" * @build         " + $.moment().format("llll") + " ET",
	" * @release       " + $.gitRevSync.long() + " [" + $.gitRevSync.branch() + "]",
	" * @copyright     Copyright (c) " + $.moment().format("YYYY") + ", <%= pkg.copyright %>",
	" *",
	" */",
	""
].join("\n");

// Settings for SVG optimization, used in other settings (see below)
let svgoOptions =  {
	plugins: [
		{ cleanupIDs: false },
		{ mergePaths: false },
		{ removeViewBox: false },
		{ convertStyleToAttrs: false },
		{ removeUnknownsAndDefaults: false },
		{ cleanupAttrs: false },
		{ inlineStyles: false }
	]
};


// Project settings
var settings = {

	browserSync: {
		proxy:'https://' + pkg.name + '.' + require('os').userInfo().username + '.localhost',
		open: false,	// Don't open browser, change to "local" if you want or see https://browsersync.io/docs/options#option-open
		notify: false,	// Don't notify on every change
		https:  {
			key: require('os').homedir() + '/server.key',
			cert: require('os').homedir() + '/server.crt'
		}
	},

	templates: {
		// Only used for browser-sync / auto-refresh when saving templates 
		src: [
			"./templates/**/*.tpl",
			"./**/*.html"
		],
		active: false
	},

	css: {
		src: './src/css/**/*.scss',
		dest: pkg.project_settings.prefix + 'css/',
		srcMain: [
			'./src/css/main.scss',
			// You can add more files here that will be built seperately,
			// f.e. newsletter.scss
		],
		options: {
			sass: {
				outputStyle: 'expanded',
				precision: 3,
				errLogToConsole: true,
			}
		},
		optionsProd: {
			sass: {
				outputStyle: 'compressed',
				precision: 3,
				errLogToConsole: true
			}
		}
	},

	/** TODO: Refactor to use module / exports etc. / modern Javascript 
	 * so we can handle it like with CSS -> multiple targets
	 */
	js: {
		src:	'./src/js/*.js',
		dest:	pkg.project_settings.prefix + 'js/',
		destFile:	'main.js'
	},

	jsModules: {
		src: 	'./src/js/modules/*.js',
		dest: 	pkg.project_settings.prefix + 'js/modules/'
	},

	jsVendor: {
		src: [
			'./src/js/vendor/**/*.js',
			'./node_modules/dialog-polyfill/dist/dialog-polyfill.js'
			// Add single vendor files here,
			// they will be copied as is to `{prefix}/js/vendor/`,
			// e.g. './node_modules/flickity/dist/flickity.pkgd.min.js',
		],
		dest:	pkg.project_settings.prefix + 'js/vendor/'
	},

	cssVendor: {
		src:	[
			'./src/css/vendor/**/*.css',
			'./node_modules/dialog-polyfill/dist/dialog-polyfill.css'
			// Add single vendor files here,
			// they will be copied as is to `{prefix}/css/vendor/`,
			// e.g. './node_modules/flickity/dist/flickity.min.css'
		],
		dest:	pkg.project_settings.prefix + 'css/vendor/'
	},

	fonts: {
		src:	'./src/fonts/**/*',
		dest:	pkg.project_settings.prefix + 'fonts/'
	},

	images: {
		src:	'./src/img/**/*',
		dest:	pkg.project_settings.prefix + 'img/',
		options: [
			$.imagemin.optipng({ optimizationLevel: 5 }),
			$.imagemin.svgo(svgoOptions)
		]
	},

	icons: {
		src:	'./src/icons/**/*.svg',
		dest:	pkg.project_settings.prefix + 'img/icons/',
		options: [
			$.imagemin.svgo(svgoOptions)
		]
	},

	sprite: {
		src: './src/icons/**/*.svg',
		dest: pkg.project_settings.prefix + 'img/',
		destFile:	'icons.svg',
		options: [
			$.imagemin.svgo(svgoOptions)
		]
	},

	favicon: {
		src: './src/img/favicon.svg',
		dest: pkg.project_settings.prefix + 'img/favicons/',
		background: '#ffffff'
	}
};



// Clean dist before building
function cleanDist() {
	return $.del([
		pkg.project_settings.prefix + '/'
	]);
}

/*
 *  Task: Compile SASS to CSS
 */
function css() {

	$.log("Building CSS" + ((isProduction) ? " [production build]" : " [development build]"));

	var stream = 
		gulp.src(settings.css.srcMain, { sourcemaps: !isProduction })
			.pipe($.plumber({ errorHandler: onError}))
	;

	var options = (isProduction) ? settings.css.optionsProd.sass : settings.css.options.sass;
	stream = stream
		.pipe($.sass(options).on('error', $.sass.logError))
		.pipe($.autoprefixer(settings.css.options.autoprefixer))
	;

	if (isProduction) {
		stream = stream.pipe($.cleanCss())
			.pipe($.header(banner, { pkg: pkg }));
	}

	stream = stream
		.pipe(gulp.dest(settings.css.dest, {
			sourcemaps: (!isProduction ? '.' : false)  
		}))
		.pipe($.browserSync.stream());

	return stream;
}

function cssVendor() {
	return gulp.src(settings.cssVendor.src)
		.pipe(gulp.dest(settings.cssVendor.dest));
}


/*
 * Task: Concat and uglify Javascript with terser
 */
function js() {

	$.log("Building Javascript" + ((isProduction) ? " [production build]" : " [development build]"));

	var stream = gulp
		.src(settings.js.src, { sourcemaps: !isProduction })
			.pipe($.jsvalidate().on('error', function(jsvalidate) { console.log(jsvalidate.message); this.emit('end'); }));


	stream = stream.pipe($.concat(settings.js.destFile));

	if (isProduction) {
		stream = stream
			// Not possible to use strip-debug for modules for the time being,
			// see issue: https://github.com/sindresorhus/strip-debug/issues/23
			// .pipe($.stripdebug())
			.pipe($.terser()).on('error', function (error) {
				if (error.plugin !== "gulp-terser-js") {
					console.log(error.message);
				}
				this.emit('end');
			})
			.pipe($.header(banner, { pkg: pkg }));
	}

	stream = stream
		.pipe(gulp.dest(settings.js.dest, {
			sourcemaps: (!isProduction ? '.' : false)
		}))
		.pipe($.browserSync.stream());

	return stream;
}

function jsModules() {
	// return gulp.src(settings.jsModules.src)
	// 	.pipe(gulp.dest(settings.jsModules.dest))
	// 	.pipe($.browserSync.stream());
	$.log("Building Javascript modules " + ((isProduction) ? " [production build]" : " [development build]"));

	var stream = gulp
		.src(settings.jsModules.src, { sourcemaps: !isProduction })
		.pipe($.jsvalidate().on('error', function(jsvalidate) { console.log(jsvalidate.message); this.emit('end'); }));

	if (isProduction) {
		stream = stream
			// Not possible to use strip-debug for modules for the time being,
			// see issue: https://github.com/sindresorhus/strip-debug/issues/23
			// .pipe($.stripdebug())
			.pipe($.terser()).on('error', function (error) {
				if (error.plugin !== "gulp-terser-js") {
					console.log(error.message);
				}
				this.emit('end');
			})
			.pipe($.header(banner, { pkg: pkg }));
	}

	stream = stream
		.pipe(gulp.dest(settings.jsModules.dest, {
			sourcemaps: (!isProduction ? '.' : false)
		}))
		.pipe($.browserSync.stream());

	return stream;
}



function jsVendor() {
	return gulp.src(settings.jsVendor.src)
		.pipe(gulp.dest(settings.jsVendor.dest));
}



function fonts() {
	return gulp.src(settings.fonts.src)
		.pipe(gulp.dest(settings.fonts.dest));
}



/*
 * Task: create images
 * TODO: Check if optimization is more effectiv when it is done separately for all different image types(png, svg, jpg)
 */
function images() {
	// optimize all other images
	// TODO: It seems that plugin in don't overwrites existing files in destination folder!??
	return gulp.src(settings.images.src)
		.pipe($.newer(settings.images.dest))
		.pipe($.imagemin(settings.images.options, { verbose: true }))
		.pipe(gulp.dest(settings.images.dest));
}



function icons() {
	return gulp.src(settings.icons.src)
		.pipe($.newer(settings.icons.dest))
		.pipe($.imagemin(settings.icons.options))
		.pipe(gulp.dest(settings.icons.dest));
}


/*
 * Task: create sprites(SVG): optimize and concat SVG icons
 */
function sprite() {
	return gulp.src(settings.sprite.src)
		.pipe($.imagemin(settings.sprite.options))
		.pipe($.svgstore({
			inlineSvg: true
		}))
		.pipe($.rename(settings.sprite.destFile))
		.pipe(gulp.dest(settings.sprite.dest));
}


/**
 * Task: Dummy task to perform reload on template change
 */
function templates(done) {
	$.browserSync.reload();
	done();
}


/*
 * Default TASK: Watch SASS and JAVASCRIPT files for changes,
 * build CSS file and inject into browser
 */
function gulpDefault(done) {

	checkKey();
	$.browserSync.init(settings.browserSync);

	gulp.watch(settings.css.src, css);
	gulp.watch(settings.js.src, js);
	gulp.watch(settings.jsModules.src, jsModules);
	
	if (settings.templates.active) {
		gulp.watch(settings.templates.src, templates);
	}
	
	done();
}


/**
 * Generate favicons
 */
function favicon() {
	return gulp.src(settings.favicon.src)
		.pipe($.favicons({
			appName: pkg.name,
			appShortName: pkg.name,
			appDescription: pkg.description,
			developerName: pkg.author,
			developerUrl: pkg.repository.url,
			background: settings.favicon.background,
			path: settings.favicon.dest,
			url: pkg.project_settings.url,
			display: "standalone",
			orientation: "portrait",
			scope: "/",
			start_url: "/",
			version: pkg.version,
			logging: false,
			pipeHTML: false,
			replace: true,
			icons: {
				android: false,
				appleIcon: false,
				appleStartup: false,
				coast: false,
				firefox: false,
				windows: false,
				yandex: false,
				favicons: true
			}
		}))
		.pipe(gulp.dest(settings.favicon.dest));
}

// Check if SSL Key exists in default Directory
function checkKey() {
	try {
		fs.accessSync(settings.browserSync.https.key, fs.constants.R_OK);
	}
	catch (err) {
		settings.browserSync.https = null;
		settings.browserSync.proxy = 'http://' + pkg.name + '.' + require('os').userInfo().username + '.localhost';
	}
}

/*
 * Task: Build all
 */
exports.build = series(cleanDist, js, jsModules, jsVendor, css, cssVendor, images, sprite, icons, fonts);

exports.default = gulpDefault;
exports.cleanDist = cleanDist;
exports.css = css;
exports.js = js;
exports.jsVendor = jsVendor;
exports.cssVendor = cssVendor;
exports.fonts = fonts;
exports.images = images;
exports.icons = icons;
exports.sprite = sprite;
exports.favicon = favicon;
exports.jsModules = jsModules;
