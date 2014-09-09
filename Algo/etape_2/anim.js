function initCanvas(canvas, context)
{
  canvas.width  = context.width;
  canvas.height = context.height;
};

function clearCanvas(ctx_object)
{
  ctx_object.ctx.clearRect(0, 0, ctx_object.width, ctx_object.height);
};

function switchImage(pos)
{
  if (pos[pos.length-1] == '1')
  {
    return pos.substring(0, pos.length-1) + '2';
  }
  else
  {
    return pos.substring(0, pos.length-1) + '1';
  }
};

function drawDog(ctx_object, image)
{
    drawDog.anim++;
    ctx_object.ctx.drawImage(image.img, image.sprites[image.pos].x,
                             image.sprites[image.pos].y, image.width,
                             image.height, drawDog.pos.x, drawDog.pos.y,
                             image.width, image.height);
};

function move(ctx_object, image, direction, cases)
{
    if (direction == 'forward')
    {
        if (drawDog.anim % 5 == 0)
        {
            image.pos = image.transition[image.pos].right;
            if (drawDog.pos.x <= 400)
            {
                drawDog.pos.x -= 5;
            }
        }
        if (drawDog.dep++ == cases)
        {
            clearInterval(drawDog.interval);
            image.pos = "wait_1";
        }
        clearCanvas(ctx_object);
        drawDog(ctx_object, image);
    }
};

function forward(ctx_object, image, cases)
{
    drawDog.dep = 0;
    drawDog.interval = setInterval(function() { move(ctx_object, image, "forward", cases * 10); }, 1000/30);
};

$(document).ready(function()
  {
    var canvas_sprite = document.getElementById("board");
    var ctx_sprite    = new Object(
      {
        'ctx': canvas_sprite.getContext('2d'),
        'width': 400,
        'height': 400
      }
    );
    var dog = new Object(
      {
        'img': new Image(),
        'width': 55,
        'height': 55,
        'pos': 'wait_1',
        'direction': 0,
        'sprites': {
          'wait_1': { 'x': 424, 'y': 4 },
          'wait_2': { 'x': 489, 'y': 4 },
          'walk_1': { 'x': 215, 'y': 4 },
          'walk_2': { 'x': 150, 'y': 4 },
          'walk_3': { 'x':  85, 'y': 4 },
          'walk_4': { 'x':  20, 'y': 4 },
        },
        'transition': {
          'wait_1': { 'left': 'walk_1', 'right': 'walk_1', 'none': 'wait_2' },
          'wait_2': { 'left': 'walk_1', 'right': 'walk_3', 'none': 'wait_1' },
          'walk_1': { 'left': 'walk_2', 'right': 'walk_2', 'none': 'wait_1' },
          'walk_2': { 'left': 'walk_1', 'right': 'walk_1', 'none': 'wait_1' },
          'walk_3': { 'left': 'walk_1', 'right': 'walk_4', 'none': 'wait_1' },
          'walk_4': { 'left': 'walk_1', 'right': 'walk_3', 'none': 'wait_1' }
        }
      }
    );

    initCanvas(canvas_sprite, ctx_sprite);
    dog.img.src = 'assort.png';
    drawDog.pos = { 'x': 175, 'y': 175 };
    drawDog.anim = 0;

    // Affichage du sprite
    clearCanvas(ctx_sprite);
    drawDog(ctx_sprite, dog);

    $("button").click(function () {
                    try
                    {
                        $("#error").hide();
                        var result = parser.parse($("textarea").val());
                        eval(result);
                        result = result.replace("\n", "<br />\n");
                        $("#error").show();
                    }
                    catch (e)
                    {
                        $("#error").html(String(e));
                        $("#error").show();
                    }
                });
  }
);
