{IF({ISSET:mostCommentedContent:VAR})}
<div id="widgetMostViewed">
	<h4>Am häufigsten kommentiert</h4>
	<ul>
		{VAR:mostCommentedContent}
	</ul>
</div>
{ENDIF}