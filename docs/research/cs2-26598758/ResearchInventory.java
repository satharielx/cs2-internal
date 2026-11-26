// Ghidra headless post-script. Read-only target inspection; project changes only.
//@category Research
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

public class ResearchInventory extends GhidraScript {
    public void run() throws Exception {
        Path out = Paths.get(getScriptArgs()[0]);
        Files.createDirectories(out);
        String name = currentProgram.getName();
        try (PrintWriter w = new PrintWriter(Files.newBufferedWriter(out.resolve(name + ".txt"), StandardCharsets.UTF_8))) {
            w.println("Program: " + name);
            w.println("Language: " + currentProgram.getLanguageID());
            w.println("Compiler: " + currentProgram.getCompilerSpec().getCompilerSpecID());
            w.println("Image base: " + currentProgram.getImageBase());
            w.println("Format: " + currentProgram.getExecutableFormat());
            w.println("SHA256: " + currentProgram.getExecutableSHA256());
            for (var block : currentProgram.getMemory().getBlocks())
                w.println("BLOCK " + block.getName() + " " + block.getStart() + " size=" + block.getSize() + " execute=" + block.isExecute());
            SymbolIterator symbols = currentProgram.getSymbolTable().getAllSymbols(true);
            List<Symbol> candidates = new ArrayList<>();
            while (symbols.hasNext()) {
                Symbol s = symbols.next();
                if (s.isExternalEntryPoint()) {
                    w.println("EXPORT " + s.getAddress() + " " + s.getName(true));
                    if (Set.of("CreateInterface", "Source2Main", "SteamAPI_Init", "SteamAPI_InitFlat", "GetSchemaSystem", "CreateResourceSystem", "InstallSchemaBindings", "GetResourceManifestCount", "GetResourceManifests", "ExtractModuleMetadata", "BinaryProperties_GetValue", "Plat_FloatTime", "MemAlloc_AllocFunc", "MemAlloc_FreeFunc").contains(s.getName()) || s.getName().startsWith("?LoadKV3@@"))
                        candidates.add(s);
                }
            }
            DecompInterface decompiler = new DecompInterface();
            try {
                decompiler.openProgram(currentProgram);
                for (Symbol s : candidates) {
                    monitor.checkCancelled();
                    Address a = s.getAddress();
                    if (!currentProgram.getMemory().contains(a) || !currentProgram.getMemory().getBlock(a).isExecute()) continue;
                    disassemble(a);
                    Function f = getFunctionAt(a);
                    if (f == null) f = createFunction(a, s.getName());
                    w.println("\nFUNCTION " + s.getName() + " " + a + " RVA=" + Long.toHexString(a.subtract(currentProgram.getImageBase())));
                    Instruction ins = getInstructionAt(a);
                    for (int i=0; ins != null && i<28; i++, ins=ins.getNext()) {
                        w.println(ins.getAddress() + " " + ins);
                        if (ins.getFlowType().isTerminal()) break;
                    }
                    if (f != null) {
                        DecompileResults result = decompiler.decompileFunction(f, 15, monitor);
                        if (result.decompileCompleted()) w.println(result.getDecompiledFunction().getC());
                        else w.println("DECOMPILER: " + result.getErrorMessage());
                    }
                }
            } finally { decompiler.dispose(); }
            w.println("\nFunction count after targeted disassembly: " + currentProgram.getFunctionManager().getFunctionCount());
        }
        println("RESEARCH_COMPLETE " + name);
    }
}
