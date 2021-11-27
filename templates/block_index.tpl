<!doctype html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7" lang="{PAGELANG}"> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8" lang="{PAGELANG}"> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9" lang="{PAGELANG}"> <![endif]-->
<!--[if gt IE 8]><!--><html class="no-js" lang="{PAGELANG}"> <!--<![endif]-->
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
		<title>{PAGETITLE} - Wolfgang Braun</title>
		<meta name="description" content="">
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<link rel="stylesheet" href="/dist/css/main.css">


		{LAYOUTMODE_STARTSCRIPT}
		{IF (!{LAYOUTMODE})}
		<!-- Add javascript libraries here -->
		{ENDIF}
	</head>
	<body>
		{INCLUDE:PATHTOWEBROOT.'dist/img/svgdefs.svg'}

		<div class="outer-container">

			{INCLUDE:PATHTOWEBROOT.'templates/partials/header.tpl'}
			
			<div class="main-content">


				<section class="page">
					<header class="block-header">
						<div class="block__mood">
							<img src="/media/{PAGEPROPERTY:image:frau_800x400.jpg:{PAGEID}}" alt="" />
						</div>
						<h1 class="block__title display-font">{PAGETITLE}</h1>
						<p class="block__leitbild display-font">{PAGEPROPERTY:description::{PAGEID}}</p>
					</header>

					<ul class="block-index">
						{LOOP NAVIGATION({PAGEID})}
							<li class="block-index__item">
								<a href="{NAVIGATION:link}">{NAVIGATION:title}</a>
							</li>
						{ENDLOOP NAVIGATION}
					</ul>
				</section>
			</div>
		</div>

		{INCLUDE:PATHTOWEBROOT.'templates/partials/bottom_scripts.tpl'}
		{LAYOUTMODE_ENDSCRIPT}
	</body>
</html>
