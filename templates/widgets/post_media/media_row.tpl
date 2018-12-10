{IF("{VAR:mediaType}"=="image")}<!--    IMAGE    -->
<div class="postMediaImage">
	<a class="colorBoxImages" href="/img/mlog/{VAR:media_file}" title="{VAR:media_title}" >
		<img src="/img/mlog/thumbnails/{VAR:media_file}" alt="{VAR:media_title}" />
	</a>
</div>
{ENDIF}

{IF("{VAR:mediaType}"=="document")}<!--    DOCUMENT    -->
<a href="/downloads/mlog/{VAR:media_file}" >{VAR:media_title}</a>
{ENDIF}

{IF("{VAR:mediaType}"=="link")}<!--    LINK    -->
<a href="{VAR:media_url}" alt="" target="_blank" />{IF({ISSET:media_url_alias:VAR})}{VAR:media_url_alias}{ELSE}{VAR:media_url}{ENDIF}</a>
{ENDIF}

{IF("{VAR:mediaType}"=="date")}<!--    LINK    -->
{VAR:media_start_date_date}{IF({ISSET:media_end_date_date:VAR})} bis {VAR:media_end_date_date}{ENDIF}: {VAR:media_title}
{ENDIF}


