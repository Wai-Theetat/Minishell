#include "minishell.h"

static void	free_split(char **split)
{
    int	i;

    if (!split)
        return ;
    i = 0;
    while (split[i])
        free(split[i++]);
    free(split);
}

static char	*join_path(const char *dir, const char *cmd)
{
    char	*tmp;
    char	*full_path;

    if (!dir || !cmd)
        return (NULL);
    if (dir[0] == '\0')
        return (ft_strdup(cmd));
    tmp = ft_strjoin(dir, "/");
    if (!tmp)
        return (NULL);
    full_path = ft_strjoin(tmp, cmd);
    free(tmp);
    return (full_path);
}

static char	*find_exec_in_paths(char **paths, char *cmd)
{
    char	*full_path;
    int		i;

    i = 0;
    while (paths[i])
    {
        full_path = join_path(paths[i], cmd);
        if (full_path && access(full_path, X_OK) == 0)
        {
            free_split(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_split(paths);
    return (NULL);
}

char	*find_exec(char *cmd, t_env *envp)
{
    char	*path_env;
    char	**paths;

    if (!cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    path_env = ft_env_get(envp, "PATH");
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    return (find_exec_in_paths(paths, cmd));
}
