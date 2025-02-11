# MAC instructions
VM1. Log in, abrir o terminal
VM2. Pegar o Ip da máquin 
    ip addr - > 172.16.168.130
VM3. Instalar o servidor SSH
    sudo apt-get install openssh-server
H1. Entrar no terminal
H2. Gerar a chave ssh
    ssh-keygen -t ed25519
    (dar enter para tudo)
H3. Copiar para VM
    ssh-copy-id uservm@172.16.168.130