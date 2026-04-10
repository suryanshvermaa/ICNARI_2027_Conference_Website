import { MapPin, Mail, Phone } from "lucide-react"; // or use any icon lib
import ContactForm from "../components/ContactForm";

const ContactSection = () => {
  return (
    <section className="bg-transparent py-19 px-4">
      <h2 className="text-3xl font-bold text-center mb-4 text-zinc-900 dark:text-slate-50">Contact Us</h2>
      <div className="w-24 h-1 bg-indigo-500 dark:bg-indigo-400/60 mx-auto mb-8 rounded-full" />

      <div className="flex flex-col gap-6 max-w-3xl mx-auto">
        {/* Location */}
        <div className="bg-indigo-600 text-white rounded-xl p-6 flex items-start gap-4 shadow-md border border-indigo-500/20 dark:bg-slate-900/40 dark:text-slate-50 dark:border-slate-700/60">
          <div className="bg-white/15 p-2 rounded-full dark:bg-indigo-500/15">
            <MapPin className="w-6 h-6" />
          </div>
          <div>
            <h3 className="font-semibold text-lg">Location:</h3>
            <p className="text-sm mt-1 leading-relaxed">
            Department of Electrical Engineering,<br></br>
            National Institute of Technology Patna,<br></br> Ashok Rajpath, Mahendru, Patna, Bihar- 800005, India
            
            </p>
          </div>
        </div>

        {/* Email */}
        <div className="bg-indigo-600 text-white rounded-xl p-6 flex items-start gap-4 shadow-md border border-indigo-500/20 dark:bg-slate-900/40 dark:text-slate-50 dark:border-slate-700/60">
          <div className="bg-white/15 p-2 rounded-full dark:bg-indigo-500/15">
            <Mail className="w-6 h-6" />
          </div>
          <div>
            <h3 className="font-semibold text-lg">Email:</h3>
            <p className="text-sm mt-1">nasl@nitp.ac.in</p>
          </div>
        </div>

        {/* Call */}
        <div className="bg-indigo-600 text-white rounded-xl p-6 flex items-start gap-4 shadow-md border border-indigo-500/20 dark:bg-slate-900/40 dark:text-slate-50 dark:border-slate-700/60">
          <div className="bg-white/15 p-2 rounded-full dark:bg-indigo-500/15">
            <Phone className="w-6 h-6" />
          </div>
          <div>
            <h3 className="font-semibold text-lg">Call:</h3>
            <p className="text-sm mt-1">+91-7840809129, +91-9635102410, +91-9304640178</p>
          </div>
        </div>
      </div>
      
         {/* Google Map */}
         <div className="mt-10 max-w-4xl mx-auto px-4">
        <iframe
        title="NIT Patna Map"
        src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3597.521975435267!2d85.1719948!3d25.6207961!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x39ed58dce6732867%3A0x4059f39a1ac82f06!2sNational%20Institute%20of%20Technology%2C%20Patna!5e0!3m2!1sen!2sin!4v1743854714737!5m2!1sen!2sin"
        width="100%"
        height="300"
        style={{border:0}}
        allowFullScreen
        loading="lazy" 
        referrerPolicy="no-referrer-when-downgrade"
        className="rounded-2xl border border-zinc-200 dark:border-slate-700/60">
        </iframe>
      </div>
      <ContactForm/>

    </section>
  );
};

export default ContactSection;