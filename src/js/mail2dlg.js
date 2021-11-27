/**
 * Mail2Dlg
 *
 * @author Johannes Braun <j.braun@agentur-halma.de>
 * @package halma
 * @version 2021-10-27
 *
 *
 * API
 *
 * ```
 * let m = new Mail2Dlg();
 * ```
 */
class Mail2Dlg {

	constructor() {
		this.selector = '[href^=mailto]';
		this.dialog = this.createEl('dialog'); //document.createElement('dialog');
		this.dialogContentArea = this.createEl('div',  'mail2dlg-content-area');
		this.dialogActionArea = this.createEl('div', 'mail2dlg-action-area');
		this.dialog.appendChild(this.dialogContentArea);
		this.dialog.appendChild(this.dialogActionArea);

		let closeBtn = document.createElement('button');
		closeBtn.innerText = 'schliessen';
		closeBtn.addEventListener('click', () => {
			this.dialog.close();
		});
		this.dialog.appendChild(closeBtn);
		document.body.appendChild(this.dialog);

		dialogPolyfill.registerDialog(this.dialog);
		let p = document.createElement('p');
		p.innerText = "Hello world.";
		this.dialog.appendChild(p);
		this.init();
	}


	createEl(tagName, className = null) {
		let el = document.createElement(tagName);
		if (className !== null) {
			el.className = className;
		}
		return el;
	}


	init() {
		var links = document.querySelectorAll(this.selector);
		for (var i = 0; i < links.length; i++) {
			links[i].addEventListener('click', (ev) => { this.onMailtoLinkClicked(ev); });
		}
		console.log(this.selector, links.length);
	}


	onMailtoLinkClicked(ev) {

		ev.preventDefault();

		let email = null;
		let href = ev.target.href;
		let match = href.match(/mailto:([a-zA-Z0-9@\!\ \#\$\%\&\'\*\+\-\/\=\?\ \^\_\`\{\|\}\~\.]+)/);
		if (match && match.length > 0) {
			email = match[1];
		}
		else {
			// TODO!
		}
		console.log(href, email);

		let actions = [
			{
				name: 'verfassen mit Standard E-Mail Programm',
				href: href
			},
			{
				name: 'verfassen mit GMail',
				href: '#to-be-implemented'
			},
			{
				name: 'E-Mail Adresse in die Zwischenablage kopieren',
				href: 'javascript:navigator.clipboard.writeText(\'' + email + '\');'
			}
		];

		this.openDialog(href, actions);
	}



	openDialog(href, actions) {
		let list = document.createElement('ul');
		actions.forEach((action) => {
			let listItem = document.createElement('li');
			let link = document.createElement('a');
			link.innerText = action.name;
			link.href = action.href;
			listItem.appendChild(link);
			list.appendChild(listItem);
		});
		this.dialogContentArea.innerHTML = '';
		this.dialogContentArea.appendChild(list);
		this.dialog.showModal();
	}
}

let m2d = new Mail2Dlg();
