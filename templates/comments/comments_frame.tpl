
	<div id="commentsEntries" class="clearfix">
		<a name="{VAR:articleId}" href="#{VAR:articleId}" class="linkAnchor"></a>
		<h5 class="commentsTitle">
		{IF("{VAR:commentsCount}" == 0)}Keine Kommentare{ENDIF}
		{IF("{VAR:commentsCount}" == 1)}Ein Leserkommentar zu diesem Artikel:{ENDIF}
		{IF("{VAR:commentsCount}" > 1)}{VAR:commentsCount} Leserkommentare zu diesem Artikel:{ENDIF}
		</h5>
	</div>
	<div id="commentsContainer">
		{VAR:commentsList}
		<a id="last_comment" href="#" class="linkAnchor"></a>
	</div>
