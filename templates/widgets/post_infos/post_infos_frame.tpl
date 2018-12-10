<div id="widgetPostInfo">
	<h4>Informationen zum Artikel</h4>
<!--
	<table class="widgetPostInfoDetails">
		{IF({ISSET:author_name:VAR})}
		<tr>
			<td class="infoTitle">Autor/in</td>
			<td><a href="/{PAGELANG}/10/{VAR:authorLinkName}.html?search={VAR:authorId}&amp;searchIn=authors">{VAR:author_prename} {VAR:author_name}</a></td>
		</tr>
		{ENDIF}
	
	{IF({ISSET:post_online_date_date:VAR})}
		<tr>
			<td class="infoTitle">Datum</td>
			<td>{VAR:post_online_date_date} / {VAR:post_online_date_hour}.{VAR:post_online_date_minute} Uhr</td>
		</tr>
	{ENDIF}

	{IF({ISSET:categoriesContent:VAR})}
		<tr>
			<td class="infoTitle">Kategorie{IF ("{VAR:categoriesNr}" > "1")}n{ENDIF}</td>
			<td>{VAR:categoriesContent}</td>
		</tr>
	{ENDIF}
		<tr>
			<td class="infoTitle">Kommentare</td>
			<td><a href="/{PAGELANG}/{PAGEID}/{VAR:linkTitle}.html?aid={VAR:postId}&amp;cp={VAR:currentPage}{IF ({ISSET:search:VAR})}&amp;search={VAR:search}{ENDIF}#last_comment">{VAR:commentsNr}</a></td>
		</tr>
		
	{IF({ISSET:tagsContent:VAR})}
		<tr>
			<td class="infoTitle">Schlagworte</td>
			<td>{VAR:tagsContent}</td>
		</tr>
	{ENDIF}
	</table>
-->
	{IF({ISSET:author_image:VAR})}<div class="widgetPostInfoAuthorImage"><img src="/img/authors/thumbnails/{VAR:author_image}" alt="{VAR:author_prename} {VAR:author_name}" /></div>{ENDIF}
	{IF({ISSET:author_name:VAR})}
		<div class="infoElement">
			<span class="infoTitle">Autor:</span>
			<a href="/{PAGELANG}/10/{VAR:authorLinkName}.html?search={VAR:authorId}&amp;searchIn=authors">{VAR:author_prename} {VAR:author_name}</a>
		</div>
		{ENDIF}
	
	{IF({ISSET:post_online_date_date:VAR})}
		<div class="infoElement">
			<span class="infoTitle">Datum:</span>
			{VAR:post_online_date_date} <!-- / {VAR:post_online_date_hour}.{VAR:post_online_date_minute} Uhr -->
		</div>
	{ENDIF}

	{IF({ISSET:categoriesContent:VAR})}
		<div class="infoElement">
			<span class="infoTitle">Kategorie{IF ("{VAR:categoriesNr}" > "1")}n{ENDIF}:</span>
			{VAR:categoriesContent}
		</div>
	{ENDIF}
		<div class="infoElement">
			<span class="infoTitle">Kommentare:</span>
			<a href="/{PAGELANG}/{PAGEID}/{VAR:linkTitle}.html?aid={VAR:postId}&amp;cp={VAR:currentPage}{IF ({ISSET:search:VAR})}&amp;search={VAR:search}{ENDIF}#last_comment">{VAR:commentsNr}</a>
		</div>
		
	{IF({ISSET:tagsContent:VAR})}
		<div class="infoElement">
			<span class="infoTitle">Schlagworte:</span>
			{VAR:tagsContent}
		</div>
	{ENDIF}
</div>