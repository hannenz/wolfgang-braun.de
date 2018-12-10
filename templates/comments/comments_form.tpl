	<div id="commentsForm" class="clearfix">
		<a name="form" href="#" class="linkAnchor"></a>
		<h5>Diesen Artikel kommentieren</h5>
		
		<form action="#last_comment" method="post" id="commentform{VAR:articleId}">
			<div class="formField">
				<label for="commentTitle">Betreff</label>
				<input id="commentTitle" type="text"  value="{VAR:commentTitle}" name="commentTitle" tabindex="{VAR:articleId}1" />
			</div>
			<div class="formField">
				<label for="commentText">Text (notwendig)</label>
				<textarea id="commentText" name="comment" tabindex="{VAR:articleId}2">{VAR:comment}</textarea>
			</div>
			<div class="formField">
				<label for="commentAuthor">Name (notwendig)</label>
				<input id="commentAuthor" name="author" class="textField" value="{VAR:author}" tabindex="{VAR:articleId}3"  type="text" />
			</div>
			<div class="formField">
				<label for="commentEmail">E-Mail Adresse (notwendig)</label>
				<input id="commentEmail" name="email" class="textField" value="{VAR:email}" size="22" tabindex="{VAR:articleId}4" type="text" />
			</div>
			<div class="formField">
				<label for="commentURL">Webseite</label>
				<input id="commentURL" type="text" value="{VAR:author_url}" name="author_url"  tabindex="{VAR:articleId}5" />
			</div>
			<div class="formField clearfix">
				<input id="commentNotify" type="checkbox" name="notify"  tabindex="{VAR:articleId}7"  {IF ({ISSET:notify:VAR})}checked="checked"{ENDIF} />
				<label for="commentNotify">Bei neuen Kommentaren per E-Mail benachrichtigen</label>
			</div>
			<div class="formField formFieldCaptcha">
				<label for="commentCaptcha">Bitte geben Sie den nebenstehenden Code ein</label>
				<input id="commentCaptcha" type="text"  name="captchaVarName"  tabindex="{VAR:articleId}6" />
				<img src="/phpincludes/captcha/captcha_image.php?sid={SID}" alt="" />
			</div>
			<div class="formButtons">
				<input name="comment_post_ID" value="{VAR:articleId}" type="hidden" />
				<input name="submit_form" value="1" type="hidden" />
				<input type="image"  name="submit" tabindex="{VAR:articleId}8"  src="/img/submit.jpg" alt="absenden" />
			</div>
		</form>
	</div>