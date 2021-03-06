/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxisimo <maxisimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:11:51 by thbernar          #+#    #+#             */
/*   Updated: 2018/11/26 21:41:19 by maxisimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	bmp_getfile_size(t_bmp *bmp, int fd)
{
	unsigned char	c;
	int				i;
	int				bytes_read;

	i = 0;
	while (i < 18 && (bytes_read = read(fd, &c, 1)) != 0)
	{
		i++;
	}
	read(fd, &c, 1);
	bmp->width = c;
	read(fd, &c, 1);
	bmp->width += c << 8;
	read(fd, &c, 1);
	bmp->width += c << 16;
	read(fd, &c, 1);
	bmp->width += c << 24;
	read(fd, &c, 1);
	bmp->height = c;
	read(fd, &c, 1);
	bmp->height += c << 8;
	read(fd, &c, 1);
	bmp->height += c << 16;
	read(fd, &c, 1);
	bmp->height += c << 24;
}

static void	bmp_getfile_data(t_bmp *bmp, int fd, int i, int b_r)
{
	unsigned char	c;
	int				size;

	i = 0;
	size = bmp->width * bmp->height * 3;
	while ((b_r = read(fd, &c, 1)) != 0 && i < size)
	{
		if (i % 3 == 0)
			bmp->data[i + 2] = c;
		if (i % 3 == 1)
			bmp->data[i] = c;
		if (i % 3 == 2)
			bmp->data[i - 2] = c;
		i++;
	}
}

static void	bmp_readfile(t_bmp *bmp, int fd)
{
	int				i;
	int				j;
	int				bytes_read;
	unsigned char	c;

	i = 0;
	j = 0;
	bytes_read = 1;
	bmp_getfile_size(bmp, fd);
	i = i + 8;
	bmp->data = (int *)malloc(sizeof(int) * bmp->width * bmp->height * 3);
	if (!bmp->data)
		exit(-1);
	while (i < 36 && read(fd, &c, 1))
	{
		i++;
	}
	bmp_getfile_data(bmp, fd, i, bytes_read);
}

void		bmp_loadfile(t_bmp *bmp, char *fname)
{
	int fd;

	if (((fd = open(fname, O_RDONLY)) < 0))
	{
		ft_putstr("Fatal error : invalid file.");
		exit(-1);
	}
	bmp_readfile(bmp, fd);
	bmp->scale = 1.0;
	ft_putstr(fname);
	ft_putstr(" loaded.\n");
}

t_color		get_pixel_color(t_bmp *img, int x, int y)
{
	int		os;
	t_color	color;

	os = ((x + y * img->width) % (img->width * img->height)) * 3;
	color.r = img->data[0 + os];
	color.g = img->data[1 + os];
	color.b = img->data[2 + os];
	return (color);
}
