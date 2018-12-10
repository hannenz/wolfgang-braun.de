<!-- Start: Artikel -->
<tr>
	<td align="center" valign="top">
		<table border="0" cellpadding="20" cellspacing="0" width="600" id="templateBody">
			<tr>
				<td valign="top" align="left" class="bodyContent">
					<div>
						<font size="5" color="#E00A0A" class="h1">{VAR:title}</font><br>
						&nbsp;<br />
						{IF({ISSET:image:VAR})}<p><img src="{VAR:image}" align="right" />{ENDIF}
							{VAR:text}
						<p>{VAR:link}</p>
					</div>
				</td>
			</tr>
		</table>
	</td>
</tr>
<!-- Ende: Artikel -->