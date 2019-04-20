mp.events.add('render', () => {
    mp.game.graphics.drawText('hacked ...', [0.5, 0.005], 
    {
      font: 4,
      color: [255, 255, 255, 255],
      scale: [1.0, 1.0],
      outline: true
    });
});
