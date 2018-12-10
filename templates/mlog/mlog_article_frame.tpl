{IF ({ISSET:contentList:VAR})}
<div class="pagingContainer pagingContainerTop">{VAR:pagingPrev} {VAR:pagingContent} {VAR:pagingNext}</div>
<div id="categoryNewsOverview clearfix">
	{VAR:contentList}
</div>
<div class="pagingContainer pagingContainerBottom">{VAR:pagingPrev} {VAR:pagingContent} {VAR:pagingNext}</div>
{ELSE}
<p>In dieser Rubrik existieren im Augenblick leider keine Artikel.</p>
{ENDIF}