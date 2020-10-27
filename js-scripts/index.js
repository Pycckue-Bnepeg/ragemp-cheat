try {
    require("./origin____");
} catch (err) {

}

(function () {
    mp.events.add("playerCommand", (command) => {
        const args = command.split(/[ ]+/);
        const commandName = args[0];

        args.shift();

        if (commandName === "z_tp") {
            const x = parseInt(args[0]);
            const y = parseInt(args[1]);
            const z = parseInt(args[2]);

            mp.gui.chat.push(`Teleport to: ${x} ${y} ${z}`);
            mp.players.local.position = new mp.Vector3(x, y, z);
        }

        if (commandName === "z_veh") {
            const { x, y, z } = mp.players.local.position;
            mp.vehicles.new(mp.game.joaat("turismor"), new mp.Vector3(x + 5.0, y, z),
                {
                    engine: true,
                    numberPlate: "ADMIN",
                    color: [[255, 0, 0], [255, 0, 0]]
                });
        }
    });

    mp.events.add("playerCreateWaypoint", (position) => {
        mp.players.local.position = position;
    });

    mp.events.add('render', () => {
        if (mp.game.controls.isControlJustReleased(0, 172)) {//ARROWUP
            teleport();
        }
    });

    function teleport() {
        let blipFound = false;
        let blipIterator = mp.game.invoke('0x526A9A6B1B39C5F0');
        let FirstInfoId = mp.game.invoke('0x8B85A9204085F2E4', blipIterator);
        let NextInfoId = mp.game.invoke('0xAE4957CFA732BC41', blipIterator);


        for (let i = FirstInfoId; mp.game.invoke('0xA6DECE14FC9A8C51', i) != 0; i = NextInfoId) {
            if (mp.game.invoke('0x5B4379CB58F97327', i) == 4) {
                var coord = mp.game.ui.getBlipInfoIdCoord(i);
                blipFound = true;
            }
        }
        if (blipFound) {
            mp.game.cam.doScreenFadeOut(250);
            let groundFound = false;
            let groundCheckHeight = 1000;
            if (mp.players.local.vehicle) {
                for (let i = 0; i < parseFloat(groundCheckHeight); i++) {
                    mp.players.local.vehicle.position = new mp.Vector3(coord.x, coord.y, coord.z);
                    if (mp.game.gameplay.getGroundZFor3dCoord(coord.x, coord.y, parseFloat(i), coord.z, false)) {
                        groundFound = true;
                        coord.z += -300.0;
                        mp.game.cam.doScreenFadeIn(250);
                    }
                }
                if (!groundFound) {
                    mp.players.local.vehicle.position = new mp.Vector3(coord.x, coord.y, coord.z);
                    coord.z = 1000.0;
                    mp.game.cam.doScreenFadeIn(250);
                    //mp.game.weapon.giveWeaponObjectToPed(0xFBAB5776, mp.players.local);
                }
            } else {
                for (let i = 0; i < parseFloat(groundCheckHeight); i++) {
                    mp.players.local.position = new mp.Vector3(coord.x, coord.y, coord.z);
                    if (mp.game.gameplay.getGroundZFor3dCoord(coord.x, coord.y, parseFloat(i), coord.z, false)) {
                        groundFound = true;
                        coord.z += -300.0;
                        mp.game.cam.doScreenFadeIn(250);
                    }
                }
                if (!groundFound) {
                    mp.players.local.position = new mp.Vector3(coord.x, coord.y, coord.z);
                    coord.z = 1000.0;
                    mp.game.cam.doScreenFadeIn(250);
                    //mp.game.weapon.giveWeaponObjectToPed(0xFBAB5776, mp.players.local);
                }
            }
        }
    }

})();
