<div class="postDetailContainer clearfix">
	<h1>{VAR:post_title}</h1>
	<h2>{VAR:post_subtitle}</h2>
	{IF({ISSET:mediaBarTop:VAR})}
	<div class="postDetailMedia">
		<div class="postDetailMediaImagesContainer clearfix">
			{VAR:contentListImage}
		</div>
	</div>
	{ENDIF}

	<div class="postDetailText hyphenate clearfix">
		{VAR:post_text}
	</div>

	{IF({ISSET:mediaBarBottom:VAR})}
	<div class="postDetailAddContent postDetailMedia">
		{IF({ISSET:contentListImage:VAR})}
			<h5>Bilderstrecke</h5>
			<div class="postDetailMediaImagesContainer clearfix">
				{VAR:contentListImage}
			</div>
		{ENDIF}
	</div>
	{ENDIF}

	{IF ("{GETVAR:tid}" == "2")}
	<div class="postDetailInfos">
		<h5>Artikelinformationen</h5>
		Rubrik <span class="category">{VAR:categoryBelongsContent}</span> / 
		{IF({ISSET:post_author:VAR})}
		von {VAR:author_prename} {VAR:author_name} / {ENDIF}
		<span class="date">{VAR:post_online_date_date}</span> 
	</div>
	{ENDIF}

	<div id="socialshareprivacyDetails" class="clearfix"></div>
	
	{VAR:furtherInformation}	
	
	<div class="postDetailNavigationContainer postDetailNavigationBottom">
		<a href="/{PAGELANG}/10/{IF({ISSET:categoryName:VAR})}{VAR:categoryName}{ELSE}Themen{ENDIF},{VAR:currentPage},{VAR:categoryId}.html{IF({ISSET:search:VAR})}?search={VAR:search}{ENDIF}{IF({ISSET:searchIn:VAR})}&amp;searchIn={VAR:searchIn}{ENDIF}#{VAR:id}" class="linkBack">zurück zur Übersicht</a>
	</div>
</div>

