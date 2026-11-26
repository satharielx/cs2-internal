//@category Research
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import java.nio.file.*;
import java.io.*;

public class InspectFunctions extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        DecompInterface decompiler = new DecompInterface();
        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(Paths.get(args[0])))) {
            decompiler.openProgram(currentProgram);
            for (int i=1; i<args.length; i++) {
                var address = currentProgram.getImageBase().add(Long.parseLong(args[i],16));
                disassemble(address);
                var function = getFunctionAt(address);
                if (function == null) function = createFunction(address, null);
                out.println("RVA " + args[i] + " " + function);
                var refs = getReferencesTo(address);
                for (var ref : refs) out.println("REFERENCE " + ref);
                if (function != null) {
                    var result = decompiler.decompileFunction(function,30,monitor);
                    if (result.decompileCompleted()) out.println(result.getDecompiledFunction().getC());
                    else out.println(result.getErrorMessage());
                }
            }
        } finally { decompiler.dispose(); }
    }
}
