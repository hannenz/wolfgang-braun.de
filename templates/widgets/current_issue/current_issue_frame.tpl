<div id="widgetCurrentIssue">
	{IF ({ISSET:title:VAR})}
		<h4 class="titleCurrentIssue">natur {VAR:publishing_month}<span>/</span>{VAR:publishing_year}: {VAR:title}</h4>
	{ENDIF}
	{IF ({ISSET:cover_image:VAR})}
		<img src="http://img.natur.de/img/issues/{VAR:cover_image}" class="imageCurrentIssueCover imageFloatRight" alt="natur {VAR:publishing_month}/{VAR:publishing_year}" />
	{ENDIF}
	<p class="hyphenate">
		{VAR:teaser}
	</p>
	{IF ({ISSET:link:VAR})}
	<p class="currentIssueReadMore">
		<a href="{VAR:link}">Mehr erfahren.</a>
	</p>
	{ENDIF}
</div>