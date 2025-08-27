Projet IRC – Documentation
1. Réseau et Sockets

Loopback / localhost
L’adresse 127.0.0.1 désigne la machine locale.
Plage valide : 127.0.0.1 → 127.255.255.255.

Port
Un port est représenté par un uint16_t (entier non signé sur 16 bits).

Bind()
Deux façons de remplir bind() :

a. Avec sockaddr_in
→ utiliser reinterpret_cast<sockaddr*>.

b. Avec addrinfo

struct addrinfo hints = {}, *res;
hints.ai_family = AF_INET;          // ou AF_UNSPEC pour IPv4+IPv6
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;        // pour bind

int err = getaddrinfo(NULL, "6667", &hints, &res);
if (err == 0)
{
    bind(fd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
}


Buffer
Gérer correctement le recv(), notamment lorsque les données reçues dépassent 1024 octets (concaténation/join nécessaire).

Client IRC
nc (netcat) agit déjà comme un client IRC. Le sujet impose uniquement d’implémenter le serveur, pas le client.

2. Contraintes et Règles

Mot de passe serveur

Longueur : 8 à 30 caractères

Doit contenir :

1 minuscule

1 majuscule

1 chiffre

1 caractère spécial parmi :

! ' - _ { } [ ] ( ) @ $ £ ? ; :


Restrictions d’implémentation

memset : interdit → utiliser une alternative.

3. Commandes IRC
3.1 Commandes utilisateur
3.1.1 NICK

Usage : attribue ou modifie le pseudonyme d’un utilisateur.

Syntaxe :

NICK <pseudo> [<hopcount>]


hopcount : utilisé uniquement entre serveurs. Ignoré si fourni par un client.

Collisions de pseudos : suppression globale du pseudo + KILL.

Erreurs possibles :

ERR_NONICKNAMEGIVEN

ERR_ERRONEUSNICKNAME

ERR_NICKNAMEINUSE

ERR_NICKCOLLISION

Exemples :

NICK Wiz
:WiZ NICK Kilroy

3.1.2 USER

Usage : enregistrement initial d’un utilisateur (nom d’utilisateur, hôte, serveur, nom réel).

Syntaxe :

USER <username> <hostname> <servername> <realname>


realname doit être en dernier, précédé de :, car il peut contenir des espaces.

Particularité : côté serveur, hostname et servername sont ignorés pour des raisons de sécurité.

Erreurs possibles :

ERR_NEEDMOREPARAMS

ERR_ALREADYREGISTRED

Exemple :

USER guest tolmoon tolsun :Ronnie Reagan
:testnick USER guest tolmoon tolsun :Ronnie Reagan

3.1.3 MODE (utilisateur)

Usage : modifie la visibilité, le statut ou les privilèges d’un utilisateur.

Syntaxe :

MODE <pseudo> [(+|-)<modes>]


Modes disponibles :

a : absent (via AWAY, pas MODE)

i : invisible

w : réception des WALLOPS

r : restreint (imposé par serveur)

o : opérateur

O : opérateur local

s : notifications serveur (obsolète)

Règles :

Impossible de s’auto-promouvoir opérateur (+o, +O → ignoré).

Impossible de retirer sa restriction (-r → ignoré).

Erreurs possibles :

ERR_NEEDMOREPARAMS

ERR_USERSDONTMATCH

ERR_UMODEUNKNOWNFLAG

Exemples :

MODE WiZ -w
MODE Angel +i
MODE WiZ -o

3.2 Commandes liées aux canaux
3.2.1 JOIN

Usage : rejoindre un ou plusieurs canaux.

Syntaxe :

JOIN <canal>{,<canal>} [<clé>{,<clé>}]


Conditions :

Invitation requise si canal est en mode invite-only.

Pas de ban actif sur l’utilisateur.

Fournir la clé correcte si définie.

Réponses en cas de succès :

RPL_TOPIC

RPL_NAMREPLY (liste des utilisateurs présents)

Erreurs possibles :

ERR_NEEDMOREPARAMS

ERR_BANNEDFROMCHAN

ERR_INVITEONLYCHAN

ERR_BADCHANNELKEY

ERR_CHANNELISFULL

ERR_BADCHANMASK

ERR_NOSUCHCHANNEL

ERR_TOOMANYCHANNELS

Exemples :

JOIN #foobar
JOIN &foo fubar
JOIN #foo,&bar fubar
JOIN #foo,#bar fubar,foobar
JOIN #foo,#bar
:WiZ JOIN #Twilight_zone

3.2.2 INVITE

Usage : inviter un utilisateur sur un canal.

Syntaxe :

INVITE <pseudo> <canal>


Seuls les membres du canal peuvent inviter, et seuls les opérateurs si le canal est en mode invite-only.

Seuls l’invitant et l’invité sont notifiés.

Erreurs possibles :

ERR_NEEDMOREPARAMS

ERR_NOSUCHNICK

ERR_NOTONCHANNEL

ERR_USERONCHANNEL

ERR_CHANOPRIVSNEEDED

Exemples :

:Angel!wings@irc.org INVITE Wiz #Dust
INVITE Wiz #Twilight_Zone

3.2.3 KICK

Usage : expulser un utilisateur d’un canal.

Syntaxe :

KICK <canal>[,<canal>] <utilisateur>[,<utilisateur>] [:commentaire]


Restrictions : un serveur ne doit pas transmettre un KICK avec plusieurs canaux/utilisateurs aux clients (compatibilité ascendante).

Erreurs possibles :

ERR_NEEDMOREPARAMS

ERR_NOSUCHCHANNEL

ERR_BADCHANMASK

ERR_CHANOPRIVSNEEDED

ERR_USERNOTINCHANNEL

ERR_NOTONCHANNEL

Exemples :

KICK &Melbourne Matthew
KICK #Finnish John :Speak English
:WiZ!jto@tolsun.oulu.fi KICK #Finnish John

3.3 Commandes de messages
3.3.1 PRIVMSG

Usage : envoyer un message privé à un utilisateur ou un canal.

Exemples et réponses :

PRIVMSG dawd
→ 401 dawd :No such nick/channel

PRIVMSG saiisako
→ 401 saiisako :No such nick/channel

PRIVMSG sdawdj,dawkuhd,ldaiwjd
→ 401 sdawdj,dawkuhd,ldaiwjd :No such nick/channel

PRIVMSG :salut
→ 401 :salut :No such nick/channel

PRIVMSG Saiisako
→ 461 PRIVMSG :Not enough parameters

PRIVMSG Saiisako salut
→ message envoyé

4. Notes diverses

Le projet doit se conformer au protocole RFC 1459 (IRC – mai 1993).

Certaines commandes ont un comportement particulier entre serveurs (notamment USER et NICK).

Les mécanismes d’invitation et de gestion des droits (MODE, INVITE, KICK) doivent être strictement respectés.
