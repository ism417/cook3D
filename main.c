/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysouhail <ysouhail@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:01:10 by ysouhail          #+#    #+#             */
/*   Updated: 2025/01/07 12:10:51 by ysouhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int ac, char **av)
{
	//
	if(ac  != 2)
		return (1);
	(void) ac;
	t_game game;
	if(check_name(av[1]) == false)
		return (1);
	// game.map = check_map(av[1], &game);
	parse_map(check_map(av[1], &game), &game);
	// game.map = &av[i+1];
	game.ply = init_ply();
	// game.map = get_map(av[1]);
	desplay_map(&game);
	mlx_loop_hook(game.mlx, ft_hook, &game);
	mlx_loop(game.mlx);
	int i = 0;
	while (game.map[i])
	{
		printf("%s\n",game.map[i]);
		free(game.map[i]);
		i++;
	}
	return (0);
	//
	// t_game *game;

	// game = malloc(sizeof(t_game));
	
	
	// for(int i = 0; i < 3; i++)
	// 	printf("%s\n", game->map[i]);
}
