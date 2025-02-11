# Guia de Configuração do VSCode para Conexão SSH com VMs Linux

Este guia tem como objetivo ajudar os alunos a configurarem o VSCode em suas máquinas locais para conectar remotamente às VMs Linux Debian utilizando o plugin **Remote - SSH** do VSCode.

## Requisitos

1. **VSCode** instalado no host (Windows ou Mac).
2. **Extensão Remote - SSH** instalada no VSCode.
3. **VM Linux Debian** funcionando:
    - VMware Workstation (para Mac)
    - VirtualBox (para Windows)

---

## Configuração para Usuários de Mac (VMware Workstation)

### Passo 1: Configurar a VM Linux Debian

1. **Logar na VM Linux** e abrir o terminal.
2. **Obter o IP da máquina virtual**:
    
    ```bash
    ip addr
    
    ```
    
    - Procure pela interface `ens33` ou `eth0` (dependendo da configuração da VM). O IP geralmente estará no formato `192.168.x.x`.
3. **Instalar o servidor SSH na VM** (se ainda não estiver instalado):
    
    ```bash
    sudo apt-get update
    sudo apt-get install openssh-server
    
    ```
    
4. **Verificar se o servidor SSH está ativo**:
    
    ```bash
    sudo systemctl status ssh
    
    ```
    
    - Se não estiver ativo, inicie o serviço:
        
        ```bash
        sudo systemctl start ssh
        
        ```
        

---

### Passo 2: Configurar o Host Mac

1. **Abrir o terminal do Mac**.
2. **Gerar uma chave SSH**:
    
    ```bash
    ssh-keygen -t ed25519
    
    ```
    
    - Pressione `Enter` para todas as opções (usar o caminho padrão e sem senha).
3. **Copiar a chave pública para a VM**:
    
    ```bash
    ssh-copy-id uservm@IP_DA_VM
    
    ```
    
    - Substitua `uservm` pelo nome de usuário da VM e `IP_DA_VM` pelo IP obtido no Passo 1.
4. **Testar a conexão**:
    
    ```bash
    ssh uservm@IP_DA_VM
    
    ```
    
    - Se conectar corretamente, prossiga para o VSCode.

---

### Passo 3: Configurar o VSCode

1. **Abrir o VSCode e instalar a extensão "Remote - SSH"** se ainda não estiver instalada.
2. **Adicionar a configuração do SSH no VSCode**:
    - Pressione `Cmd+Shift+P` e escolha **Remote-SSH: Open SSH Configuration File**.
    - Adicione as seguintes linhas no arquivo que abrir:
        
        ```
        Host minha-vm-linux
            HostName IP_DA_VM
            User uservm
            IdentityFile ~/.ssh/id_ed25519
        
        ```
        
3. **Conectar à VM pelo VSCode**:
    - Pressione `Cmd+Shift+P` e escolha **Remote-SSH: Connect to Host** > `minha-vm-linux`.
4. **Pronto!** Agora você está conectado à sua VM pelo VSCode.

---

## Configuração para Usuários de Windows (VirtualBox)

### Passo 1: Configurar a VM Linux Debian

1. **Logar na VM Linux** e abrir o terminal.
2. **Instalar o servidor SSH na VM**:
    
    ```bash
    sudo apt-get update
    sudo apt-get install openssh-server
    
    ```
    
3. **Verificar se o servidor SSH está ativo**:
    
    ```bash
    sudo systemctl status ssh
    
    ```
    
    - Se não estiver ativo, inicie o serviço:
        
        ```bash
        sudo systemctl start ssh
        
        ```
        

### Passo 2: Configurar o Port Forwarding no VirtualBox

1. **Desligar a VM** antes de configurar.
2. **Abrir o VirtualBox**, selecionar a VM, clicar em **Configurações > Rede**.
3. **Certificar-se que a rede está configurada como NAT**.
4. **Configurar o redirecionamento de portas**:
    - Clique em **Avançado > Redirecionamento de Porta**.
        - (ou Redes > Expert > Port Forward)
    - Adicione uma nova regra:
        - **Nome**: SSH
        - **Protocolo**: TCP
        - **Porta do host**: 2222
        - **Endereço do convidado**: 10.0.2.15 (IP padrão NAT)
        - **Porta do convidado**: 22
5. **Iniciar a VM** novamente.
 
---

### Passo 3: Configurar o Host Windows

1. **Abrir o Prompt de Comando ou PowerShell**.
2. **Gerar a chave SSH**:
    
    ```powershell
    ssh-keygen -t ed25519
    
    ```
    
    - Pressione `Enter` para todas as opções.
3. **Método 1: Copiar a chave pública manualmente para a VM**:
    - Acesse o diretório onde a chave foi gerada (geralmente `C:\\Users\\SeuUsuario\\.ssh\\id_ed25519.pub`).
    - Copie o conteúdo do arquivo `id_ed25519.pub`.
4. **Adicionar a chave pública na VM (manual)**:
    - No terminal da VM, execute:
        
        ```bash
        mkdir -p ~/.ssh
        echo "cole_aqui_o_conteúdo_da_chave_pública" >> ~/.ssh/authorized_keys
        chmod 600 ~/.ssh/authorized_keys
        chmod 700 ~/.ssh
        
        ```
        
    - Substitua `cole_aqui_o_conteúdo_da_chave_pública` pelo conteúdo copiado da chave.
5. **Método 2: Copiar a chave pública via PowerShell**:
    - No **PowerShell**, execute o comando abaixo para enviar a chave diretamente para a VM:
        
        ```powershell
        type $env:USERPROFILE\\.ssh\\id_ed25519.pub | ssh -p 2222 uservm@localhost "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
        
        ```
        
    - Substitua `uservm` pelo nome de usuário da VM.
6. **Testar a conexão**:
    
    ```powershell
    ssh -p 2222 uservm@localhost
    
    ```
    
    - Substitua `uservm` pelo nome de usuário da VM.

---

### Passo 4: Configurar o VSCode

1. **Abrir o VSCode e instalar a extensão "Remote - SSH"** se ainda não estiver instalada.
2. **Adicionar a configuração do SSH no VSCode**:
    - Pressione `Ctrl+Shift+P` e escolha **Remote-SSH: Open SSH Configuration File**.
    - Adicione as seguintes linhas:
        
        ```
        Host minha-vm-linux
            HostName localhost
            Port 2222
            User uservm
            IdentityFile ~/.ssh/id_ed25519
        
        ```
        
3. **Conectar à VM pelo VSCode**:
    - Pressione `Ctrl+Shift+P` e escolha **Remote-SSH: Connect to Host** > `minha-vm-linux`.
4. **Pronto!** Agora você está conectado à sua VM pelo VSCode.

---

## Possíveis Problemas e Soluções

1. **Permissões da chave SSH**:
    - Se houver erro de permissão, ajuste as permissões da pasta `.ssh`:
        
        ```bash
        chmod 700 ~/.ssh
        chmod 600 ~/.ssh/authorized_keys
        
        ```
        
2. **Firewall bloqueando o SSH**:
    - Verifique se o firewall está bloqueando a porta 22:
        
        ```bash
        sudo ufw allow ssh
        sudo ufw enable
        
        ```
        
3. **Verificar IP da VM (para Mac)**:
    - Se o IP mudar após reiniciar a VM, atualize o arquivo de configuração do SSH.
4. **Conexão recusada no Windows**:
    - Certifique-se que o redirecionamento de portas está configurado corretamente e que a VM está ativa.

---

## Recursos úteis

- [Documentação oficial do Remote - SSH](https://code.visualstudio.com/docs/remote/ssh)
- [Como configurar redirecionamento de portas no VirtualBox](https://www.virtualbox.org/manual/ch06.html)
- [Guia do SSH no Linux](https://linuxize.com/post/how-to-enable-ssh-on-ubuntu-20-04/)
- [Como configurar SSH sem senha no Windows](https://superuser.com/questions/104257/how-to-set-up-ssh-keys-on-windows)
- [O que é um túnel SSH e como funciona](https://www.ssh.com/academy/ssh/tunneling)
