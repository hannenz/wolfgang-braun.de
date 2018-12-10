<div class="textAdContainer clearfix">
	<div class="textAdMarker">Anzeige</div>
	{IF({ISSET:adLinkIsValid:VAR})}<a href ="{PAGEURL:44}?ad={VAR:id}&amp;url={VAR:ad_link}" target="{VAR:adLinkTarget}">
	<!-- {VAR:ad_link} -->{ENDIF}
	{IF ({ISSET:ad_image:VAR})}<img src="http://img.natur.de/img/ads/{VAR:ad_image}" alt="" />{ENDIF}
	{IF ({ISSET:ad_text:VAR})}<h6>{VAR:ad_title}</h6>{ENDIF}
	{IF ({ISSET:ad_text:VAR})}<p>{VAR:ad_text}</p>{ENDIF}
	{IF({ISSET:adLinkIsValid:VAR})}</a>{ENDIF}
</div>