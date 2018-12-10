<div id="widgetPostMedia">
	<h4>Materialien zum Artikel</h4>

	{IF({ISSET:contentListDocument:VAR})}
	<div class="infoElement">
		<span class="infoTitle">Dokumente:</span>
		{VAR:contentListDocument}
	</div>
	{ENDIF}
	
	{IF({ISSET:contentListLink:VAR})}
	<div class="infoElement">
		<span class="infoTitle">Links:</span>
		{VAR:contentListLink}
	</div>
	{ENDIF}
	
	{IF({ISSET:contentListDate:VAR})}
	<div class="infoElement">
		<span class="infoTitle">Termine:</span>
		{VAR:contentListDate}
	</div>
	{ENDIF}

</div>