{IF({ISSET:mostViewedContent:VAR})}
<div id="widgetMostViewed">
	<h4>{IF({ISSET:categoryTitle:VAR})}Thema {VAR:categoryTitle}: meistgelesen{ELSE}Meistgelesen{ENDIF}</h4>
	<ul>
		{VAR:mostViewedContent}
	</ul>
</div>
{ENDIF}