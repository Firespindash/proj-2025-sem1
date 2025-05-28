function abrirModal() {
    document.getElementById("modal").style.display = "block";
}

function fecharModal() {
    document.getElementById("modal").style.display = "none";
}

// Fechar ao clicar fora do conteúdo
window.onclick = function(event) {
    const modal = document.getElementById("modal");
    if (event.target === modal) {
    	fecharModal();
    }
}

window.addEventListener('scroll', function() {
	const header = document.querySelector('header');
	if (window.scrollY > 10) {
		header.classList.add('scroll');
	} else {
		header.classList.remove('scroll');
	}
});

