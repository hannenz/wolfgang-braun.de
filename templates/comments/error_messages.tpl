<div class="errorMessage">
{IF ('{VAR:errorNr}' == '3001')}
<p>Die von Ihnen angegebene E-Mailadresse ist ung&uuml;ltig.</p>
{ENDIF}

{IF ('{VAR:errorNr}' > '3004' && '{VAR:errorNr}' <= '3005')}
	Wegen eines technischen Fehlers konnte Ihr Kommentar leider nicht bearbeitet werden (Fehler-Nr.: {VAR:errorNr})
{ENDIF}

{IF ('{VAR:errorNr}' == '3015')}
<p>Bitte geben Sie Ihren Vornamen an.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '3012')}
<p>Fehler: Bitte f&uuml;llen Sie alle erforderlichen Felder aus!</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '3016')}
<p>Fehler: Bitte f&uuml;llen Sie alle erforderlichen Felder aus!</p>
{ENDIF}


{IF ('{VAR:errorNr}' == '3013')}
<p>Fehler: Bitte geben Sie einen Kommentartext ein.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '3033')}
<p>Fehler: Kommentar f&uuml;r unbekannten Artikel nicht erlaubt.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '3017')}
<p>Aus Sicherheitsgr&uuml;nden ist ein erneutes Schreiben eines Kommentars erst in einigen Minuten m&ouml;glich.</p>
{ENDIF}


{IF ('{VAR:errorNr}' == '3014')}
<p>Bitte geben Sie den nebenstehenden Zahlencode richtig ein.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '3019')}
<p>Ihr Kommentartext ist zu lang.</p>
{ENDIF}


{IF ('{VAR:errorNr}' == '10')}
<p class="confirmMessage">Der Kommentar wurde aktiviert.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '20')}
<p>Der Kommentar ist bereits aktivert.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '30')}
<p>Der Kommentar kann nicht aktivieren werden.</p>
{ENDIF}




{IF ('{VAR:errorNr}' == '40')}
<p class="confirmMessage">Kommentar gelöscht.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '50')}
<p>Kommentar schon gelöscht.</p>
{ENDIF}

{IF ('{VAR:errorNr}' == '60')}
<p>Kommentar nicht gelöscht.</p>
{ENDIF}

</div>
{VAR:messageNr}
