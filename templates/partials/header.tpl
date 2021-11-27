			<header class="main-header">
				<div class="test">
					<h1><a href="/"><svg class="icon"><use xlink:href="#icon-quill"></use></svg> Wolfgang Braun</a></h1>
					<nav class="main-nav">
						<ul>
							{LOOP NAVIGATION(1)}
								<li {IF("{NAVIGATION:id}" == "{PAGEID}")}class="is-current"{ENDIF}>
									<a href="{NAVIGATION:link}" class="">{NAVIGATION:title}</a>
									{IF("{NAVIGATION:children}" > "0")}
										<ul>
											{LOOP NAVIGATION({NAVIGATION:id})}
												<li {IF("{NAVIGATION:id}" == "{PAGEID}")}class="is-current"{ENDIF}>
													<a href="{NAVIGATION:link}" title="{NAVIGATION:title}">{NAVIGATION:title}</a>
												</li>
											{ENDLOOP NAVIGATION}
										</ul>
									{ENDIF}
								</li>
							{ENDLOOP NAVIGATION}
						</ul>
					</nav>
					<a href="mailto:wilk.braun@online.de">wilk.braun@online.de</a>
				</div>
			</header>
