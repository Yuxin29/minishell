#include "parsing.h"
#include "minishell.h"

//redirections.c
//after parsing, we got already cmd list.
//we check here if there are redirections symbles here in each cmd.

void        check_and_apply_redirections(t_cmd *cmd);
{
    if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			exit(1);  //is exit status 1 when infile faild???
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
    }
    if (cmd->outfile)
    {
        if(cmd->append_out == 1) //append
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND);
        else if (cmd->append_out == 0)
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC);
        if (fd < 0)
        {
            perror(cmd->outfile)
            exit(1);
        }
        dup2(fd, STOUT_FILENO);
		close(fd);
    }
}


void        check_and_apply_heredocs(t_cmd *cmd_list);
{
    while (cmd_list && cmd_list->next)
    {
        if (cmd_list->heredoc_delim)
        {
            //Create a unique temporary file path to store heredoc input.

            //Open the temporary file for writing, handle errors if any.

            //Repeatedly prompt the user for input lines (e.g., with readline("> ")).

            //Stop reading when user inputs a line exactly matching the heredoc delimiter.

            //Write all lines except the delimiter into the temporary file.

            //Close the file.

            //Update the command’s infile field to the temporary file path, replacing any previous infile.

            //Free and clear heredoc_delim since it's no longer needed.

            //Move to the next command until all heredocs processed.
        }
        cmd_list = cmd_list->next;
    }
}
