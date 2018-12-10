<a class="pagingLink prevPage" href="/{PAGELANG}/{VAR:pageId}/{IF({ISSET:categoryName:VAR})}Rubrik-{VAR:categoryName}{ELSE}Themen{ENDIF},{VAR:prevPage},{VAR:categoryId}.html{IF({ISSET:search:VAR})}?search={VAR:search}{ENDIF}{IF({ISSET:searchIn:VAR})}&amp;searchIn={VAR:searchIn}{ENDIF}">&lt;</a> /
{SPLITTEMPLATEHERE}
<a class="pagingLink {IF({ISSET:selected:VAR})}linkSelected{ENDIF}" href="/{PAGELANG}/{VAR:pageId}/{IF({ISSET:categoryName:VAR})}Rubrik-{VAR:categoryName}{ELSE}Themen{ENDIF},{VAR:listItem},{VAR:categoryId}.html{IF({ISSET:search:VAR})}?search={VAR:search}{ENDIF}{IF({ISSET:searchIn:VAR})}&amp;searchIn={VAR:searchIn}{ENDIF}">{VAR:listItem}</a> /
{SPLITTEMPLATEHERE}
<a class="pagingLink nextPage" href="/{PAGELANG}/{VAR:pageId}/{IF({ISSET:categoryName:VAR})}Rubrik-{VAR:categoryName}{ELSE}Themen{ENDIF},{VAR:nextPage},{VAR:categoryId}.html{IF({ISSET:search:VAR})}?search={VAR:search}{ENDIF}{IF({ISSET:searchIn:VAR})}&amp;searchIn={VAR:searchIn}{ENDIF}">&gt;</a> 
{SPLITTEMPLATEHERE}
<a class="pagingLink firstPage" href="/{PAGELANG}/{VAR:pageId}/News-Aktuelles-{VAR:firstPage}.html?cp={VAR:firstPage}">ERSTE SEITE</a>
{SPLITTEMPLATEHERE}
<a class="pagingLink lastPage" href="/{PAGELANG}/{VAR:pageId}/News-Aktuelles-{VAR:lastPage}.html?cp={VAR:lastPage}">LETZTE SEITE</a>