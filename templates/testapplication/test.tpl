{IF ("{VAR:test}" == "2")}
	<p style="color: green">Test ist 2</p>{IF ("{VAR:test1}" != "3")}<p>Und test1 ist nicht 3!</p>{ENDIF} 
{ELSE}
	<p style="color: red">Test ist <b>nicht</b> 2</p>
{ENDIF}

{EVAL}
	$content = "<p>Eval ist das!</p>";
{ENDEVAL}

<ul>
{LOOP NAVIGATION()}
	<li><a href="{NAVIGATION:link}">{NAVIGATION:title}</a></li>
{ENDLOOP NAVIGATION}
</ul>
{SWITCH ("{VAR:test}")}
	{CASE ("1")}
		<p><b>Die Zahl ist 1</b></p>
		{BREAK}
	{CASE ()}
		<p>Default: Die Zahl ist nicht  1</p>
		{BREAK}
{ENDSWITCH}