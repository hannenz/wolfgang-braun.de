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
		<script src="/dist/js/mail2dlg.js"></script>

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

				{IF("{PAGECHILDREN}" > "0")}
					<ul class="cards">
						{LOOP NAVIGATION({PAGEID})}
							<li class="card">
								<a href="{NAVIGATION:link}">
									<figure>
										<img src="/media/{PAGEPROPERTY:image:frau_800x400.jpg:{NAVIGATION:id}}" alt="" />
									</figure>

									<h2>{NAVIGATION:title}</h2>
									<p>{PAGEPROPERTY:description::{NAVIGATION:id}}</p>
								</a>
							</li>
						{ENDLOOP NAVIGATION}
					</ul>
				{ELSE}
					<section class="page lyric">
						<article>				
							{LOOP CONTENT(1)}{ENDLOOP CONTENT}
						</article>
					</section>
				{ENDIF}
			</div>
		</div>

		{INCLUDE:PATHTOWEBROOT.'templates/partials/bottom_scripts.tpl'}
		{LAYOUTMODE_ENDSCRIPT}
	</body>
</html>
