<div class="postOverviewContainer clearfix">
	<a id="{VAR:id}" name="{VAR:id}" href="#"></a>
	<h3><a href="/{PAGELANG}/20/{VAR:linkTitle},{VAR:currentPage},{VAR:categoryId},{VAR:id}.html{IF({ISSET:search:VAR})}?search={VAR:search}{ENDIF}{IF({ISSET:searchIn:VAR})}&amp;searchIn={VAR:searchIn}{ENDIF}">{VAR:post_title}</a></h3>
	<h4>{VAR:post_subtitle}</h4>
	{IF ({ISSET:post_image:VAR})}<img src="http://img.natur.de/img/mlog/static/{VAR:post_image}" alt="" class="postOverviewImage" />{ENDIF}
	<p class="postTeaser clearfix hyphenate">
		{VAR:post_teaser} ... <a href="/{PAGELANG}/20/{VAR:linkTitle},{VAR:currentPage},{VAR:categoryId},{VAR:id}.html{IF({ISSET:search:VAR})}?search={VAR:search}{ENDIF}{IF({ISSET:searchIn:VAR})}&amp;searchIn={VAR:searchIn}{ENDIF}" class="linkReadOn">weiterlesen</a>
	</p>
</div> 