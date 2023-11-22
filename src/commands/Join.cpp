#include "Command.hpp"

irc::Join::Join(Server* server) : Command(server){}
irc::Join::~Join(){}

// syntax: JOIN <channels> [<keys>]

/** 
 * The JOIN command is used by a user to request to start listening to
   the specific channel.
 *
 * @param   <channel> *( "," <channel> ) <user> *( "," <user> )
               [<comment>]
 * @return   nothing
 */

void    irc::Join::execute(User* user, std::vector<std::string> args)
{
    if (args.empty())
    {
        user->reply(ERR_NEEDMOREPARAMS(user->getNickName(), "JOIN"));
        return;
    }

    std::string user_name = args[0];
	std::string pass;
	if (args.size() > 1)
		pass = args[1];
	else
		pass = "";

    Channel *channel = user->getChannel();
	if (channel) 
    {
		user->reply(ERR_TOOMANYCHANNELS(user->getNickName(), user_name));
		return;
	}

    channel = _server->getChannel(user_name);
	if (!channel)
		channel = _server->createChannel(user_name, pass, user);

    if (channel->getLimit() > 0 && channel->getSize() >= channel->getLimit())
    {
		user->reply(ERR_CHANNELISFULL(user->getNickName(), user_name));
		return;
	}

    if (channel->getKey() != pass)
    {
		user->reply(ERR_BADCHANNELKEY(user->getNickName(), user_name));
		return;
	}

	user->join(channel);
}