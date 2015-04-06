import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.IOException;
import java.security.*;

public class worker {
	
	public static String create_hash(String value) throws NoSuchAlgorithmException{
		MessageDigest md;
		md= MessageDigest.getInstance("SHA-512");
		md.update(value.getBytes());
		byte[] mb = md.digest();
		String out = "";
		for (int i = 0; i < mb.length; i++) {
            byte temp = mb[i];
            String s = Integer.toHexString(new Byte(temp));
            while (s.length() < 2) {
                s = "0" + s;
            }
            s = s.substring(s.length() - 2);
            out += s;
        }
		return out;
		
	}
	
	public static String pseudonimize(String value) throws NoSuchAlgorithmException{
		int dot = value.indexOf('.');
		String prefix = value.substring(0, dot+1);
		String tohash = value.substring(dot+1);
		String hexdigest = worker.create_hash(tohash);
		String suffix = hexdigest.substring(0, tohash.length());
		return prefix + suffix;
		
	}
	public static long process(String fname) throws ParserConfigurationException, SAXException, IOException, NoSuchAlgorithmException, TransformerException{
		long startTime = System.currentTimeMillis();
		File fXmlFile = new File(fname);
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
		Document doc = dBuilder.parse(fXmlFile);
		
		Node root = doc.getFirstChild();
		NodeList list = root.getChildNodes();
		for(int i=0; i < list.getLength(); ++i){
			Element item = (Element)list.item(i);
			NodeList fields = item.getChildNodes();
			for(int j=0; j < fields.getLength(); ++j){
				Element field = (Element)fields.item(j);
				String name = field.getAttribute("name");
				if(name.equals("origin")){
					String v = field.getTextContent();
					String nn = worker.pseudonimize(v);
					field.setTextContent(nn);
					break;
				}
			}
			
		}
		long endTime = System.currentTimeMillis();
		long elapsed = endTime - startTime;
		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
		transformer.setOutputProperty(OutputKeys.INDENT, "yes"); 
		DOMSource source = new DOMSource(doc);
		StreamResult result = new StreamResult(new File(fname + ".java.anon"));
		transformer.transform(source, result);
		return elapsed;
	}
	/**
	 * @param args
	 * @throws IOException 
	 * @throws SAXException 
	 * @throws ParserConfigurationException 
	 * @throws NoSuchAlgorithmException 
	 * @throws TransformerException 
	 */
	public static void main(String[] args) throws ParserConfigurationException, SAXException, IOException, NoSuchAlgorithmException, TransformerException {
		// TODO Auto-generated method stub
		long startTime = System.currentTimeMillis();
		String fname = "../access_log.1.xml";
		long ptime = worker.process(fname);
		long elapsed = System.currentTimeMillis() - startTime;
		System.out.println(String.valueOf(ptime) + "\t" + String.valueOf(elapsed));
		
	}

}
