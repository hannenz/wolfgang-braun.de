<footer class="page-footer">
	{IF(!{LAYOUTMODE})}
	<nav class="pagination">
		<ul>
			{IF("{HASPREVSIBLING}" == "1")}<li class="prev">&larr; {PREVSIBLING}</li>{ENDIF}
			{IF("{HASNEXTSIBLING}" == "1")}<li class="next">{NEXTSIBLING} &rarr;</li>{ENDIF}
		</ul>
		<a class="back-link" href="/{PAGELANG}/{PARENTID}/{PARENTSLUG}">Zurück zur Übersicht</p>
	</nav>
	{ENDIF}
</footer>
